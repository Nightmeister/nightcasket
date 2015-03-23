#include "container.h"

void* cache_file(char* _filename)
{

	return 0;
}
char* read_file(char* _filename, char* _containerName,size_t &_count) /* function return data of the file */
{
	/* Define pointers and loc memory */
	FILE * cFile;
	struct header *h = (header*)malloc(sizeof(header));
	struct file *files = 0;
	char* answer;
	/* end define */

	cFile = fopen(_containerName,"rb");
	fread(h,sizeof(header),1,cFile);
	uint id = ID;
	if (h->id == id)
	{
		fseek(cFile,h->offset,SEEK_SET);
		files = (file*)calloc(h->files,sizeof(file));
		for(size_t i=0;i<h->files;i++)
		{
			fread(&files[i],sizeof(file),1,cFile);
			if (!strncmp(files[i].name,_filename,strlen(files[i].name)))
			{
				answer = (char*)calloc(files[i].length,sizeof(char));
				fseek(cFile,files[i].offset,SEEK_SET);
				_count = fread(answer,sizeof(char),files[i].length,cFile);

				/* Clean memory */
				fclose(cFile);
				free(files);
				free(h);

				return answer;
			}
		}
	} else {
		printf("Bad format");
		fclose(cFile);
	    free(h);
		return 0;
	}

	/* Clean memory */
	fclose(cFile);
	free(h);
	free(files);
	/* end clean */
	return 0;
}
size_t write_file(char* _filename,char* _containerName,char* _data, uint &_datasize) /* function write data to the file */
{
	FILE * cFile;
	struct header *h = (header*)calloc(1,sizeof(header));
	struct file *files = 0;

	cFile = fopen(_containerName,"r+b");
	uint id = ID;
	fread(h,sizeof(header),1,cFile);
	if (h->id != id) { return -1; }
	files = (file*)calloc(h->files+1,sizeof(file));
	fseek(cFile,h->offset,SEEK_SET);
	if (h->files != 0)
	{
		fread(files,sizeof(file),h->files,cFile);
	}

	strcpy(files[h->files].name,_filename);
	files[h->files].offset = h->offset;
	files[h->files].length = _datasize;

	h->files++; /* inc(file count) */

	fseek(cFile,h->offset,SEEK_SET); /* Move to end of DATA */

	fwrite(_data,_datasize,1,cFile); 
	h->offset += _datasize; /* shift offset to the new end of DATA */

	rewind(cFile); /* Move to begin */

	fwrite(h,sizeof(header),1,cFile);

	fseek(cFile,h->offset,SEEK_SET); /* move to the end of data */
	fwrite(files,sizeof(file),h->files,cFile);

	fclose(cFile);
	free(h);
	free(files);

	return 0;
}
bool create_empty_container(char* _containerName) /* Create empty container */
{
	FILE * cFile;
	struct header *h = (header*)malloc(sizeof(header));
	cFile = fopen(_containerName,"wb");
	h->id = ID;
	h->offset = sizeof(header);
	h->files = 0;
	fwrite(h,sizeof(header),1,cFile);

	fclose(cFile);
	free(h);

	return 1;
}
file* file_list(size_t &countOut,char* _containerName) /* function getting file list and returning fully structure, don't forget clean memory of _list pointer */
{
	/* Define pointers and loc memory */
	FILE * cFile;
	char* answer = "NO";
	struct header *h = (header*)calloc(1,sizeof(header));
	struct file *_file = 0;
	/* end define */

	cFile = fopen(_containerName,"rb");
	if (cFile == 0) { return 0; };
	fread(h,sizeof(header),1,cFile);
	uint id = ID; 
	if (h->id == id)
	{
		if (h->files != 0)
		{
			_file = (file*)calloc(h->files,sizeof(file));
			fseek(cFile,h->offset,SEEK_SET);
			fread(_file,sizeof(file),h->files,cFile);
		} else {
			printf("Container is empty");
			fclose(cFile);
		    free(h);
			return 0;
		}
	} else {
		printf("Bad format");
		fclose(cFile);
		free(h);
		return 0;
	}

	fclose(cFile);
	countOut = h->files;
	free(h);
	return _file;
}

/* Our functions */
void char_to_char(char* out, char* in)
{
	for(size_t i=0;i<strlen(in);i++)
	{
		out[i] = in[i];
	}
}
char* encrypt_data(char* in)
{
	char* out;
	for(size_t i=0;i<strlen(in);i++)
	{
		out[i] = in[i];
		out[i] ^= 7083113;
		out[i] ^= 3113807;
	}
	if(strlen(out) != 0) 
	{
		return out;
	} else {
		return "EMPTY";
	}
}
char* decrypt_data(char* in)
{
	char* out;
	for(size_t i=0;i<strlen(in);i++)
	{
		out[i] = in[i];
		out[i] ^= 3113807;
		out[i] ^= 7083113;
	}
    if(strlen(out) != 0) 
	{
		return out;
	} else {
		return "EMPTY";
	}
}
