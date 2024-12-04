#pragma once

// THIS WAVE FILE CREATER SOURCES COMING FROM A TUTORIAL VIDEO: https://www.bilibili.com/video/BV1xA411Q76y/?vd_source=b0d154d074af70a07354b1bd14eb46ab&spm_id_from=333.788.videopod.sections

typedef struct WAV_HEADER_S
{
	char   			    riffType[4]{'R','I','F','F'};	    
	unsigned int   	riffSize;		                      
	char   			    waveType[4]{'W','A','V','E'};	    
	char   			    formatType[4]{'f','m','t',' '};	 
	unsigned int	  formatSize{16};		               
	unsigned short	compressionCode{1};               
	unsigned short 	numChannels;	                   
	unsigned int   	sampleRate;		                  
	unsigned int   	bytesPerSecond;	                  
	unsigned short 	blockAlign;		                   
	unsigned short 	bitsPerSample;	                
	char   			    dataType[4]{'d','a','t','a'};	   
	unsigned int   	dataSize;	       	                
}WAV_HEADER;

WAV_HEADER CreateWaveHeader(int numChannels = 1,                    
                            unsigned int sampleRate = 44100,        
                            unsigned short bitsPerSample = 16);     