#ifndef container_h
#define container_h
#define ID 0x07083113;

/* Windows 32 */
#ifdef _WIN32

#define _CRT_SECURE_NO_WARNINGS
#define EXPORT extern "C" __declspec (dllexport)
#include <Windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/* End */

#else
/* Linux */
#define EXPORT

#endif

#include <memory>

typedef unsigned int uint;


struct header
{
	uint id;
	uint offset;
	uint files;
};
struct file
{
	char name[56];
	uint offset;
	uint length;
};
void* cache_file(char*);
/* Read File function. Return pointer and _count(number of bytes)
*  Don't forget free memory after using!
***********************************************************************/
EXPORT char* read_file(char* _filename,char* _containerName,size_t &_count);

/* Write File function. Return -1 if it have error
***********************************************************************/
EXPORT size_t write_file(char* _filename,char* _containerName,char* _data,uint &_datasize);

/* Get File List fuction. Return pointer and countOut(number of files)
***********************************************************************/
EXPORT file* file_list(size_t &countOut,char* _containerName);

/* Create empty container with header
***********************************************************************/
EXPORT bool create_empty_container(char* _containerName);

/* Our functions */

/* Encrypt block of char. Return pointer
***********************************************************************/
char* encrypt_data(char* in);

/* Decrypt block of char. Return pointer
***********************************************************************/
char* decrypt_data(char* in);

#endif
