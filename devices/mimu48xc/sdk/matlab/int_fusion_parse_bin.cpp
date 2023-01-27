/*
 * image_sample_points.cpp
 *
 *  Created on: 10 apr 2012
 *      Author: Johan
 */


#include "mex.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static const int MAX_FILENAME_SIZE = 1024; 
const char* check_input_output(int nlhs, mxArray *plhs[ ], int nrhs, const mxArray *prhs[]);
unsigned short calculate_checksum( unsigned char *data, int length );
unsigned short read_checksum( unsigned char *data );

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	bool print_errors = true;

	// Check that input and output conforms to intended usage
	const char* error_msg = check_input_output(nlhs, plhs, nrhs, prhs);
	if(error_msg)
		mexErrMsgTxt(error_msg);

	const int nof_imus = *reinterpret_cast<unsigned char*>( mxGetData(prhs[1]) );
	const int PAYLOAD_SIZE = 1;
	const int PACKET_SIZE = 4+PAYLOAD_SIZE+2; //8+12*nof_imus;

	size_t filename_size = mxGetNumberOfElements( prhs[0] );
	char* filename = new char[2*filename_size];
	size_t filename_length = mxGetString( prhs[0], filename, (int)( 2 * filename_size ) );

//	mexPrintf( filename );
	FILE* fd = fopen( filename, "rb" );
	if(fd == 0){
//		plhs[0] = mxCreateDoubleMatrix( 0, 0, mxREAL);
        plhs[0] = mxCreateNumericMatrix( 0, 0, mxSINGLE_CLASS ,mxREAL);
	//	plhs[1] = mxCreateNumericMatrix( 0, 0, mxSINGLE_CLASS ,mxREAL);
      //  plhs[2] = mxCreateNumericMatrix( 0, 0, mxSINGLE_CLASS ,mxREAL);
     //   plhs[3] = mxCreateNumericMatrix( 0, 0, mxSINGLE_CLASS ,mxREAL);
   //     plhs[4] = mxCreateNumericMatrix( 0, 0, mxSINGLE_CLASS ,mxREAL);
     //   plhs[5] = mxCreateNumericMatrix( 0, 0, mxSINGLE_CLASS ,mxREAL);
        //plhs[6] = mxCreateNumericMatrix( 0, 0, mxSINGLE_CLASS ,mxREAL);
	//	plhs[1] = mxCreateNumericMatrix( 0, 0, mxUINT32_CLASS ,mxREAL);
		plhs[2] = mxCreateNumericMatrix( 0, 0, mxUINT8_CLASS ,mxREAL);
		delete[] filename;
		return;
	}

	//const int nof_data_values = 24;
	//const int data_values_size = nof_data_values;

	struct stat st;
	stat(filename, &st);
	const int max_elems = st.st_size / PACKET_SIZE;
	unsigned char* fused_data = new unsigned char[ max_elems * 1 ];
	//unsigned char* fused_data2 = new unsigned char[ max_elems * 4 ];
//	unsigned char* fused_data3 = new unsigned char[ max_elems * 4 ];
//	unsigned char* fused_data4 = new unsigned char[ max_elems * 4 ];
//	unsigned char* fused_data5 = new unsigned char[ max_elems * 4 ];
//	unsigned char* fused_data6 = new unsigned char[ max_elems * 4 ];
	//unsigned char* time_data = new unsigned char[ max_elems * 4 ];
	unsigned char* raw_data = new unsigned char[ max_elems * PAYLOAD_SIZE ];
	int elem_count = 0;

	unsigned char* read_buffer = new unsigned char[ PACKET_SIZE ];

	int bytes_read = fread( read_buffer, 1, PACKET_SIZE, fd );
	while( !feof( fd )){
		if( *read_buffer == 0xAA && calculate_checksum( read_buffer, PACKET_SIZE-2 ) == read_checksum( read_buffer + PACKET_SIZE - 2 ) ){
			
			memcpy(fused_data + elem_count * 24, read_buffer + 4, 1 );
		//	memcpy(fused_data2 + elem_count * 4, read_buffer + 12, 4 );
		//	memcpy(fused_data3 + elem_count * 4, read_buffer + 16, 4 );
		//	memcpy(fused_data4 + elem_count * 4, read_buffer + 20, 4 );
		//	memcpy(fused_data5 + elem_count * 4, read_buffer + 24, 4 );
        //    memcpy(fused_data6 + elem_count * 4, read_buffer + 28, 4 );
		//	memcpy(time_data + elem_count * 4, read_buffer + 2 + 2, 4 );
			memcpy(raw_data + elem_count * PAYLOAD_SIZE, read_buffer + 2 + 2, PAYLOAD_SIZE );
			elem_count++;
			bytes_read = fread( read_buffer, 1, PACKET_SIZE, fd );
//			mexPrintf( "Read complete buffer after success %u %u\n", ftell(fd),bytes_read );
		}else{
			mexPrintf( "*" );

			//if not correct we search for a new header in the buffer
			//header=0xc2
			//Search index, 0-index if probably header so we start on 1 to find new header
			bool read_new_buffer = true;
			for (int i=1; i < PACKET_SIZE ;i++){
				//If we find header we make a new read operation from there
				if ( 0xAA == read_buffer[i] ){
					//Move array components
					for (int j=0;j+i<PACKET_SIZE;j++){
						read_buffer[j]=read_buffer[i+j];}
					//Read "i" new data to get a complete set of 31 bytes
					bytes_read = fread( read_buffer + PACKET_SIZE - i , 1, i, fd );
//					mexPrintf("Read part of buffer %u %u\n",bytes_read,i);
					//Timestamp for data
					read_new_buffer = false;
					break;
				}
			}
			//If no new header were found read complete new buffer
			if ( read_new_buffer ){
				bytes_read = fread( read_buffer, 1, PACKET_SIZE, fd );
//				mexPrintf("Read complete buffer %u\n",bytes_read);
			}
		}
	}

	plhs[0] = mxCreateNumericMatrix( 1, elem_count, mxSINGLE_CLASS ,mxREAL);
	//plhs[1] = mxCreateNumericMatrix( 1, elem_count, mxSINGLE_CLASS ,mxREAL);
//	plhs[2] = mxCreateNumericMatrix( 1, elem_count, mxSINGLE_CLASS ,mxREAL);
	//plhs[3] = mxCreateNumericMatrix( 1, elem_count, mxSINGLE_CLASS ,mxREAL);
//	plhs[4] = mxCreateNumericMatrix( 1, elem_count, mxSINGLE_CLASS ,mxREAL);
//	plhs[5] = mxCreateNumericMatrix( 1, elem_count, mxSINGLE_CLASS ,mxREAL);
//	plhs[1] = mxCreateNumericMatrix( 1, elem_count, mxUINT32_CLASS ,mxREAL);
	plhs[2] = mxCreateNumericMatrix( PAYLOAD_SIZE, elem_count, mxUINT8_CLASS ,mxREAL);
	unsigned char* calibrated = reinterpret_cast< unsigned char* >(mxGetData(plhs[0]));
//	unsigned char* output_ay = reinterpret_cast< unsigned char* >(mxGetData(plhs[1]));
//	unsigned char* output_az = reinterpret_cast< unsigned char* >(mxGetData(plhs[2]));
//	unsigned char* output_gx = reinterpret_cast< unsigned char* >(mxGetData(plhs[3]));
//	unsigned char* output_gy = reinterpret_cast< unsigned char* >(mxGetData(plhs[4]));
//	unsigned char* output_gz = reinterpret_cast< unsigned char* >(mxGetData(plhs[5]));
//	unsigned char* time_stamps = reinterpret_cast< unsigned char* >(mxGetData(plhs[1]));
	unsigned char* output_raw = reinterpret_cast< unsigned char* >(mxGetData(plhs[2]));

    const int size_inertial_data = 24 * elem_count;
	for( int i=0; i < size_inertial_data; i+=24 ){
		for (int j=0; j<24; j+=2) {
			output_mat[i+j] = inertial_data[i+j+1];
			output_mat[i+j+1] = inertial_data[i+j];
		}
	}
	/*for(int i=0; i<elem_count*4; i+=4){
		output_ax[i+0]=fused_data1[i+3];
		output_ax[i+1]=fused_data1[i+2];
		output_ax[i+2]=fused_data1[i+1];
		output_ax[i+3]=fused_data1[i+0];
	}
	for(int i=0; i<elem_count*4; i+=4){
		output_ay[i+0]=fused_data2[i+3];
		output_ay[i+1]=fused_data2[i+2];
		output_ay[i+2]=fused_data2[i+1];
		output_ay[i+3]=fused_data2[i+0];
	}
	for(int i=0; i<elem_count*4; i+=4){
		output_az[i+0]=fused_data3[i+3];
		output_az[i+1]=fused_data3[i+2];
		output_az[i+2]=fused_data3[i+1];
		output_az[i+3]=fused_data3[i+0];
	}
	for(int i=0; i<elem_count*4; i+=4){
		output_gx[i+0]=fused_data4[i+3];
		output_gx[i+1]=fused_data4[i+2];
		output_gx[i+2]=fused_data4[i+1];
		output_gx[i+3]=fused_data4[i+0];
	}
	for(int i=0; i<elem_count*4; i+=4){
		output_gy[i+0]=fused_data5[i+3];
		output_gy[i+1]=fused_data5[i+2];
		output_gy[i+2]=fused_data5[i+1];
		output_gy[i+3]=fused_data5[i+0];
	}
	for(int i=0; i<elem_count*4; i+=4){
		output_gz[i+0]=fused_data6[i+3];
		output_gz[i+1]=fused_data6[i+2];
		output_gz[i+2]=fused_data6[i+1];
		output_gz[i+3]=fused_data6[i+0];
	}*/
	for(int i=0; i<elem_count*4; i+=4){
		time_stamps[i+0]=time_data[i+3];
		time_stamps[i+1]=time_data[i+2];
		time_stamps[i+2]=time_data[i+1];
		time_stamps[i+3]=time_data[i+0];
	}
	for(int i=0;i<elem_count*PAYLOAD_SIZE;i++)
		output_raw[i]=raw_data[i];

	fclose(fd);
	delete[] fused_data1;
	delete[] fused_data2;
	delete[] fused_data3;
	delete[] fused_data4;
	delete[] fused_data5;
	delete[] fused_data6;
	delete[] time_data;
	delete[] raw_data;
	delete[] read_buffer;
	delete[] filename;
}



const char* check_input_output(int nlhs, mxArray *plhs[ ], int nrhs, const mxArray *prhs[ ]){
	if( nlhs != 8 )
		return "exactly EIGHT outputs required: , fused_data1, fused_data2, fused_data3, fused_data4, fused_data5, fused_data6, time_stamps and raw data";
	if( nrhs != 1 )
		return "one inputs required, filename";
	if( !mxIsChar( prhs[0] ) )
		return "input should be a string containing file name";
	/*if( ! ( mxGetNumberOfElements( prhs[1] ) == 1 ) || !mxIsUint8( prhs[1] ) )
		return "input should be the number of IMUs, a scalar integer";*/
	return reinterpret_cast<const char*>(0);
}


unsigned short calculate_checksum( unsigned char *data, int length ) {
	unsigned short sum=0;
	while (length-- > 0){
        	sum += *(data++);
	}
	return sum; 
}
unsigned short read_checksum( unsigned char *data ){
	typedef union{ unsigned char bytes[2]; unsigned short value;} conv_data;
	conv_data val;
	val.bytes[0] = data[1];
	val.bytes[1] = data[0];
	return val.value;
}

