#include "stdafx.h"
#include "SimulatorEx.h"
#include "PeerInfo.h"
#include "ContentInfo.h"
#include <math.h>

#define SIMULATOR_THREAD_COUNT					8
//#define DUMP

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Job
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CJob
{
public:
	CJob() {Reset();}

	void										Reset() {memset(this, 0, sizeof(*this));}
	CAtlString									GetString()
	{
		unsigned int GroupCount = 0;
		if(GroupSize > 0) GroupCount = (PeerCount + (GroupSize - 1)) / GroupSize;

		CAtlString String;
		String.Format("CacheMode=%s, GroupMode=%s, PeerCount=%u, SearchContentCount=%u, GroupSize=%u, GroupCount=%u, TTL=%u",
			CacheMode == MODE_CACHE_OFF ? "OFF" : "ON",
			GroupMode == MODE_GROUPING_OFF ? "OFF" : "ON",
			PeerCount,
			SearchContentCount,
			GroupSize,
			GroupCount,
			TTL
		);

		return String;
	}

	int											JobKind;
	int											PeerKind;
	int											RepeatIndex;

	unsigned int								RandomSeed;

	ECacheMode									CacheMode;
	EGroupMode									GroupMode;

	unsigned int								PeerCount;
	unsigned int								SearchContentCount;
	unsigned int								GroupSize;
	int											TTL;
};
CAtlList<CJob>									JobList;
unsigned int									JobIndex = 0, JobTotal = 0;
CwLock											JobListLock;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Statistics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const unsigned int								PeerCounts[] = {100, 500, 1000, 5000, 10000};
const unsigned int								JobKinds = 3;
const unsigned int								RepeatCount = 100;
class CStatistics
{
public:
	CStatistics() {memset(this, 0, sizeof(*this));}

	unsigned int								TotalUpdated;

	unsigned int								TotalMessageCount;
	unsigned int								TotalSuccess;
	unsigned int								TotalFailure;
	unsigned int								TotalHops;
};
CStatistics										Statistics[JobKinds][_countof(PeerCounts)];
CwLock											StatisticsLock[JobKinds][_countof(PeerCounts)];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI SimulatorThread(LPVOID Argument)
{
	int ThreadID = (int)Argument;
	printf("Thread %u> started.\n", ThreadID);

	CSimulatorEx *Sim;
	CJob Job;
	unsigned int JobIndex;
	while(true)
	{
		// 작업을 하나 꺼냄
		{
			CwLockAuto JobListLockAuto(&JobListLock);
			if(JobList.IsEmpty() == true) break;
			Job = JobList.RemoveHead();
			JobIndex = ++::JobIndex;
		}

		Sim = new CSimulatorEx();

		// 시뮬레이터 환경 설정
		unsigned int GroupCount = 0;
		if(Job.GroupSize > 0) GroupCount = (Job.PeerCount + (Job.GroupSize - 1)) / Job.GroupSize;
		Sim->Reset(Job.RandomSeed);
		Sim->SetVerbose(false);
		Sim->SetEnvironmentRandomly();
		Sim->SetMode(Job.CacheMode, Job.GroupMode);
		if(Job.GroupMode == MODE_GROUPING_ON)
		{
			Sim->SetGroupMaxMemeberNumber(Job.GroupSize);
			Sim->SetContentInfoFloodingTTL(Job.TTL);
		}

#ifdef DUMP
		Sim->DumpOpen();
#endif

		DWORD StartTime = GetTickCount();

		// 피어 삽입
		for(unsigned int i = 0;i < Job.PeerCount / 5;i++) Sim->InsertWorkInsertPeer(i + 1, 5);
		Sim->SimulateToInfinity();

		// 컨텐트 찾기
		CContentInfo *ContentInfo = Sim->GetRandomContent();
		for(unsigned int i = 0;i < Job.SearchContentCount;i++)
		{
			Sim->InsertWorkSearchContent(Sim->GetStep() + 1, SIM_RANDOM_VALUE, ContentInfo->ContentID);
			Sim->SimulateToInfinity();
		}

		DWORD EndTime = GetTickCount();

#ifdef DUMP
		Sim->DumpFinal();
#endif

		// 로그 출력
		printf("Thread %u> Job=(%u/%u), %s <%.1fsec>\n", ThreadID, JobIndex, JobTotal, Job.GetString(), (EndTime - StartTime) / 1000.0);

		// 각 결과 출력
		/*char FileName[MAX_PATH];
		sprintf(FileName, "Statistics/[Statistics] %s (%03d).log", Job.GetString(), Job.RepeatIndex);
		FILE *File = fopen(FileName, "w");
		if(File != NULL)
		{
			fprintf(File, "*** Simulation is terminated at step %u\n\n", Sim->GetStep());
			fprintf(File, "*** Simulation Statistics\n");
			fprintf(File, "%s\n", Sim->GetStatistics());

			fprintf(File, "*** Simulation Output\n");
			fprintf(File, "%s\n", Sim->GetLog());

			fclose(File);
		}*/

		// 전체 통계 계산
		{
			CwLockAuto StatisticsLockAuto(&StatisticsLock[Job.JobKind][Job.PeerKind]);
			Statistics[Job.JobKind][Job.PeerKind].TotalUpdated++;
			Statistics[Job.JobKind][Job.PeerKind].TotalMessageCount+= Sim->GetStatisticsTotalMessageCount();
			Statistics[Job.JobKind][Job.PeerKind].TotalSuccess+= Sim->GetStatisticsTotalSearchContentSuccessCount();
			Statistics[Job.JobKind][Job.PeerKind].TotalFailure+= Sim->GetStatisticsTotalSearchContentCount() - Sim->GetStatisticsTotalSearchContentSuccessCount();
			Statistics[Job.JobKind][Job.PeerKind].TotalHops+= Sim->GetStatisticsTotalSearchContentHopCount();

			if(Statistics[Job.JobKind][Job.PeerKind].TotalUpdated == RepeatCount)
			{
				char FileName[MAX_PATH];
				sprintf(FileName, "Statistics/[Statistics] %s (___).log", Job.GetString());
				FILE *File = fopen(FileName, "w");
				if(File != NULL)
				{
					fprintf(File, "*** Simulation Statistics\n");
					fprintf(File, "Repeat Count : %u\n", RepeatCount);
					fprintf(File, "Average Message Count (Traffic) : %g\n", (double)Statistics[Job.JobKind][Job.PeerKind].TotalMessageCount / RepeatCount);
					fprintf(File, "Total Search Content Success/Failure : %u / %u (%g%%)\n", Statistics[Job.JobKind][Job.PeerKind].TotalSuccess, Statistics[Job.JobKind][Job.PeerKind].TotalFailure, (double)Statistics[Job.JobKind][Job.PeerKind].TotalSuccess / (Statistics[Job.JobKind][Job.PeerKind].TotalSuccess + Statistics[Job.JobKind][Job.PeerKind].TotalFailure) * 100);
					fprintf(File, "Average Number of Hops per Search Content Success : %g\n", (double)Statistics[Job.JobKind][Job.PeerKind].TotalHops / Statistics[Job.JobKind][Job.PeerKind].TotalSuccess);

					fclose(File);
				}
			}
		}

		delete Sim;
	}

	printf("Thread %u> terminated.\n", ThreadID);

	return ThreadID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	//GlobalRandomSeed = GetTickCount();

	//unsigned int RandomSeed = GetTickCount();
	unsigned int RandomSeed = 0;

	CJob Job;

	// For 상일
	/*Job.Reset();
	Job.RandomSeed = RandomSeed;
	Job.Index = 1;
	Job.CacheMode = MODE_CACHE_OFF;
	Job.GroupMode = MODE_GROUPING_OFF;
	Job.PeerCount = 100;
	Job.SearchContentCount = 10;
	Job.GroupSize = 10;
	JobList.AddTail(Job);*/

	// For 광의
	CreateDirectory("Statistics", NULL);
	for(int Repeat = 0;Repeat < RepeatCount;Repeat++)
	{
		for(int i = 0;i < _countof(PeerCounts);i++)
		{
			// Pure P2P
			Job.Reset();
			Job.JobKind = 0;
			Job.PeerKind = i;
			Job.RepeatIndex = Repeat + 1;
			Job.RandomSeed = RandomSeed + Repeat;
			Job.CacheMode = MODE_CACHE_OFF;
			Job.GroupMode = MODE_GROUPING_OFF;
			Job.PeerCount = PeerCounts[i];
			Job.SearchContentCount = 2;
			JobList.AddTail(Job);

			// Key Paper
			Job.Reset();
			Job.JobKind = 1;
			Job.PeerKind = i;
			Job.RepeatIndex = Repeat + 1;
			Job.RandomSeed = RandomSeed + Repeat;
			Job.CacheMode = MODE_CACHE_ON;
			Job.GroupMode = MODE_GROUPING_OFF;
			Job.PeerCount = PeerCounts[i];
			Job.SearchContentCount = 2;
			JobList.AddTail(Job);

			// Our Work
			Job.Reset();
			Job.JobKind = 2;
			Job.PeerKind = i;
			Job.RepeatIndex = Repeat + 1;
			Job.RandomSeed = RandomSeed + Repeat;
			Job.CacheMode = MODE_CACHE_ON;
			Job.GroupMode = MODE_GROUPING_ON;
			Job.PeerCount = PeerCounts[i];
			Job.SearchContentCount = 2;
			Job.GroupSize = 10;
			Job.TTL = 6;
			JobList.AddTail(Job);
		}
	}
	JobTotal = JobList.GetCount();

	DWORD StartTime = GetTickCount();

	// Create simulator threads
	HANDLE hThread[SIMULATOR_THREAD_COUNT];
	for(int i = 0;i < SIMULATOR_THREAD_COUNT;i++)
	{
		hThread[i] = CreateThread(NULL, 0, SimulatorThread, (LPVOID)(i + 1), 0, NULL);
	}

	for(int i = 0;i < SIMULATOR_THREAD_COUNT;i++)
	{
		WaitForSingleObject(hThread[i], INFINITE);
	}

	DWORD EndTime = GetTickCount();

	printf("\nSimulation is finished. <%.1fsec>\n", (EndTime - StartTime) / 1000.0);

	//while(true) Sleep(1000);

	return 0;
}

BOOL WINAPI DllMain(HANDLE hInstance, ULONG ulReason, LPVOID lpReserved)
{
	return TRUE;
}
