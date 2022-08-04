#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Short function to count the number of lines in the file to allocate arrays to the correct length
int line_counter(FILE *fp){
	int c;
	int count = 1;

	for(c=getc(fp); c!=EOF; c=getc(fp)){
		if(c=='\n')
			count+=1;
	}
	return(count);	
}



int main() {

	float q2 = pow(1.602e-19,2);
	float me = 9.103e-31;
	float eps = 8.854e-12;
	float pi = M_PI;

	int line_count;
	int file_num = 10;

	char file_name[100];

	sprintf(file_name,"fort.%d",file_num);
	printf("%s\n",file_name);




// ***********************************************************
//	March 3 2021 UT 11
	//iritest_(&file_num);
 	iritest_();
	FILE *IRIOUT;
	IRIOUT = fopen(file_name,"r");
	if(IRIOUT == NULL) {
		printf("Error opening file.");
		exit(1);
	}

	line_count = line_counter(IRIOUT) -1;
	rewind(IRIOUT);
	printf("%d\n",line_count);
	float alt_entry;
	int Ne_entry;
	float alt[line_count];
	int Ne[line_count];
	float f[line_count];

	float tmp1, tmp12;
	int tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	int tmp11, tmp13;

	printf("%d\n",line_count);
	// Load data from file
	for(int i = 0; i<line_count; i++){
		fscanf(IRIOUT,"%f %d %f %d %d %d %d %d %d %d %d %d %d %f %d",\
		 &alt_entry, &Ne_entry, &tmp1, &tmp2, &tmp3, &tmp4, &tmp5, &tmp6,\
			&tmp7, &tmp8, &tmp9, &tmp10, &tmp11, &tmp12, &tmp13);
		alt[i] = alt_entry;
		Ne[i] = Ne_entry;
		f[i] = sqrt(Ne[i]*1e6*q2/(4*pi*eps*me))/1e6;		
		printf("%d %d %d %f \n",i,line_count,Ne[i],alt[i]);
	}
	fclose(IRIOUT);

	FILE *gnuplot;
	gnuplot = popen("gnuplot -persist","w");
	if(!gnuplot){
		perror("popen");
		exit(EXIT_FAILURE);
	}
	
	fprintf(gnuplot,"set ylabel 'Altitude (km)'\n"
			"set xlabel 'Plasma Frequency (MHz)'\n"
			"set title 'EDP For Mar 3 2021 UT 11:00:00'\n"
			"set xrange [0:12]\n"
			"set yrange [0:600]\n");
	fprintf(gnuplot, "plot '-' u 1:2 t 'EDP' w lp\n");		
	fflush(gnuplot);
    for (int i = 0; i < line_count; ++i) {
        fprintf(gnuplot,"%f %f\n", f[i], alt[i]);	
    }
	fprintf(gnuplot,"quit\n");
	pclose(gnuplot);
// ***************************************************************************
//	March 4 2021 UT 23

	// file_num +=1;
	// sprintf(file_name,"fort.%d",file_num);
	// iritest_(&file_num);
	
	// line_count = 0;
	// printf("%s\n",file_name);	
	// IRIOUT = fopen(file_name,"r");
	// if(IRIOUT == NULL) {
	// 	printf("Error opening file.");
	// 	exit(1);
	// }

	// line_count = line_counter(IRIOUT) -1;
	// rewind(IRIOUT);
	// printf("%d\n",line_count);	
	// // float alt_entry;
	// // int Ne_entry;
	// // float alt[line_count];
	// // int Ne[line_count];
	// // float f[line_count];

	// // float tmp1, tmp12;
	// // int tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	// // int tmp11, tmp13;

	// // Load data from file
	// for(int i = 0; i<line_count; i++){
	// 	fscanf(IRIOUT,"%f %d %f %d %d %d %d %d %d %d %d %d %d %f %d",\
	// 	 &alt_entry, &Ne_entry, &tmp1, &tmp2, &tmp3, &tmp4, &tmp5, &tmp6,\
	// 		&tmp7, &tmp8, &tmp9, &tmp10, &tmp11, &tmp12, &tmp13);
	// 	alt[i] = alt_entry;
	// 	Ne[i] = Ne_entry;
	// 	f[i] = sqrt(Ne[i]*1e6*q2/(4*pi*eps*me))/1e6;		
	// 	printf("%d %f\n", Ne[i],alt[i]);
	// }

	// fclose(IRIOUT);


	// FILE *gnuplot2;
	// gnuplot2 = popen("gnuplot -persist","w");
	// if(!gnuplot2){
	// 	perror("popen");
	// 	exit(EXIT_FAILURE);
	// }

	// fprintf(gnuplot2,"set ylabel 'Altitude (km)'\n"
	// 		"set xlabel 'Plasma Frequency (MHz)'\n"
	// 		"set title 'EDP For Mar 4 2021 UT 23:00:00'\n"
	// 		"set xrange [0:12]\n");
	// fprintf(gnuplot2, "plot '-' u 1:2 t 'EDP' w lp\n");		
	// fflush(gnuplot2);

    // for (int i = 0; i < line_count; ++i) {
    //     fprintf(gnuplot,"%f %f\n", f[i], alt[i]);	
    // }

	// fprintf(gnuplot2,"quit\n");
	// pclose(gnuplot2);
	
	return 0;
}