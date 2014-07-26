#include <stdio.h>
#include <unistd.h>
#include <mikmod.h>
#include <sndfile.h>
#include <libgen.h>

//extern signed short ** VC1_GetSamples();
//extern signed short ** VC2_GetSamples();

void usage(char * p_program)
{
	printf("Usage: %s [filename]\n", basename(p_program));
	printf("\nSupported formats:\n\n%s\n", MikMod_InfoLoader());
	printf("\nSupported drivers:\n\n%s\n", MikMod_InfoDriver());
}

int extract(const char * p_filename)
{
	MODULE * module;
	int i;
	signed short ** s = NULL;
	
 	module = Player_Load(p_filename, 64, 0);
	if (!module) {
		printf("Player_Load: %s\n", MikMod_strerror(MikMod_errno));
		return 1;
	}

	printf("Filename:        %s\n",    p_filename);
	printf("Songname:        %s\n",    module->songname);
	printf("No. of channels: %d\n",    module->numchn);
	printf("No. of insts:    %d\n",    module->numins);
	printf("No. of samps:    %d\n",    module->numsmp);
	
	// Init MikMod Interop
	printf("SampleExIOP:     ");
	
	s = VC1_GetSamples();
	if(!s) {
		s = VC2_GetSamples();
		if(!s) {
			printf("Error\n");
			return 2;
		}
		printf("VC2 (HQ)\n");
	} else {
		printf("VC1 (Default)\n");
	}
	
	printf("Saving Samples ...\n");
	for(i = 0; i < module->numsmp; i++) {
		char fn[256];
		SF_INFO sfi;
		SNDFILE * sf;
				
		if(!module->samples[i].length) continue;
				
		sprintf(fn, "%02d.wav", i);
		
		memset(&sfi, 0, sizeof(SF_INFO));
		sfi.samplerate = 22500;
		sfi.channels = module->samples[i].flags & SF_STEREO ? 2 : 1;
		sfi.format = SF_FORMAT_WAV;
		if(module->samples[i].flags & SF_16BITS) {
			sfi.format |= SF_FORMAT_PCM_16;
		} else {
			sfi.format |= SF_FORMAT_PCM_S8;
		}
		
		sf = sf_open(fn, SFM_WRITE, &sfi);
		
		sf_write_short(sf, s[module->samples[i].handle], (int)module->samples[i].length);
		
		sf_close(sf);
		
		printf("%03d: %-20s (%06d nsmps [* = %p]) | %02d | %c | %s | %s\n", 
			i, 
			module->samples[i].samplename, 
			(int)module->samples[i].length, 
			s[module->samples[i].handle],
			module->samples[i].flags & SF_16BITS     ? 16 : 8,
			module->samples[i].flags & SF_STEREO     ? 'S' : 'M', 
			module->samples[i].flags & SF_BIG_ENDIAN ? "BE" : "LE",
			module->samples[i].flags & SF_LOOP       ? "Loop" : "NoLoop");
		
		/**
		 * Test RAW Output
		 
		int j;
		FILE * f;
		char fn[256];
				
		sprintf(fn, "%02d.raw", i);
		f = fopen(fn, "wb");
		for(j = 0; j < module->samples[i].length; j++) {
			if(module->samples[i].inflags & SF_16BITS) {
				signed short * sss = (signed short *)s[module->samples[i].handle];
				
				if(module->samples[i].inflags & SF_STEREO) {
					fwrite(&sss[j], 2, 1, f);
				} else {
					fwrite(&sss[j], 1, 1, f);
				}
			} else {
				signed char * ssc = (signed char *)s[module->samples[i].handle];
				
				fwrite(&ssc[(j * 2)+1], 1, 1, f);
				if(module->samples[i].inflags & SF_STEREO) {
					j++;
					fwrite(&ssc[(j * 2)+1], 1, 1, f);
				}
			}
		}
		fclose(f);
		 */
		 
		
	}
	
	
	Player_Free(module);
	
	
	return 0;
}

int main(int p_argc, char * p_argv[])
{
	MikMod_RegisterAllLoaders();
//    MikMod_RegisterAllDrivers();
	MikMod_RegisterDriver(&drv_nos);
	
	if(p_argc < 2) {
		usage(p_argv[0]);
		return 1;
	}
	
	if(MikMod_Init("")) {
		printf("MikMod_Init: %s\n", MikMod_strerror(MikMod_errno));
		return 2;
	}
	
	extract(p_argv[1]);
	
	MikMod_Exit();
	
	return 0;
}
