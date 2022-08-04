// Script to sort and filter data files of the format found in AU930_ROAM.txt
// Simply run the script in the same directory as the data file which needs 
// to be sorted and filtered

#include <stdlib.h>
#include <stdio.h>



// Short function to skip the column label and blank line
int skip_line(FILE *fp){
	int c;
	while (c = fgetc(fp), c!= '\n' && c!=EOF);
	
	return c;
}

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

// Function to check for errors in input data and set the value to -1, a value 
// outside the normal data range to ensure median filtering works
float error_check(char entry[120]){
	float value;

	if(entry[0] != '-'){
		value = atof(entry);
	}
	else{
		value = -1;
	}
	return(value);
}

// Function to perform median filtering
float median_filter(float sort1, float sort2, float sort3){
	float median;
	float med_sort[3];
	float swap;
	int jMin;

	med_sort[0] = sort1;
	med_sort[1] = sort2;
	med_sort[2] = sort3;
	for(int i = 0; i<2; i++){
		jMin = i;
		for(int j = i+1; j<3; j++){
			if(med_sort[j] < med_sort[jMin]){
				jMin = j;
			}
		}
			
		if(jMin !=i){
		
			swap = med_sort[i];
			med_sort[i] = med_sort[jMin];
			med_sort[jMin] = swap;

		}	
	}
	median = med_sort[1];

	return(median);
}

// Program to scan, sort, and apply median filtering to AU930.txt
int main() {
	
	//  Main File Pointers for Input and Output
	FILE *AU930;
	FILE *AU930_filtered;
	

	int line_count;
	int line_length = 120;

	// Data and Time Values Taken From Data file
	int year_entry, month_entry, date_entry, day_entry, hour_entry, minute_entry, second_entry;

	// Entries from Data file, read into char variables to account for missing entries
	char c_entry[line_length];
	char fof2_entry[line_length], fof1_entry[line_length], foe_entry[line_length], foes_entry[line_length];
	char hes_entry[line_length],hmf2_entry[line_length], hmf1_entry[line_length],hme_entry[line_length];
	char b0_entry[line_length], b1_entry[line_length], d1_entry[line_length];


	// Open data file to be sorted and filtered
	char file_name[16] = "AU930_ROAM.txt";
	AU930 = fopen(file_name,"r");
	if(AU930 == NULL)	{
		printf("Error!");   
		exit(1);             
	}
	
	// Count Lines And Return to the top of the Input File
	line_count = line_counter(AU930)-2;
	rewind(AU930);	
	skip_line(AU930);
	skip_line(AU930);

	// Data arrays for All Input Data
	int year[line_count], month[line_count], date[line_count], day[line_count];
	int hour[line_count], minute[line_count], second[line_count], date_rank[line_count];
	int index_original[line_count];
	int int_swap, sec_min;
	float swap;
	float c_score[line_count];
	float fof2[line_count], fof1[line_count], foe[line_count], foes[line_count];
	float hes[line_count],hmf2[line_count], hmf1[line_count], hme[line_count];
	float b0[line_count], b1[line_count],d1[line_count];


	// Temporary Data Arrays for Median Filtering
	float fof2_median_sorter[line_count],fof1_median_sorter[line_count],foe_median_sorter[line_count],foes_median_sorter[line_count];
	float hes_median_sorter[line_count], hmf2_median_sorter[line_count],hmf1_median_sorter[line_count],hme_median_sorter[line_count];
	float b0_median_sorter[line_count], b1_median_sorter[line_count], d1_median_sorter[line_count];

	// Load data from file and correct for missing numerical data 
	for(int i = 0; i<line_count; i++){
		fscanf(AU930,"%d.%d.%d (%d) %d:%d:00 %s %s %s %s %s %s %s %s %s %s %s",\
			&year_entry,&month_entry,&date_entry,&day_entry,&hour_entry,&minute_entry,\
			c_entry,fof2_entry,fof1_entry,foe_entry,foes_entry,\
			hes_entry,hmf2_entry,hmf1_entry,hme_entry,b0_entry,b1_entry);

		year[i] = year_entry;
		month[i] = month_entry;
		date[i] = date_entry;
		day[i] = day_entry;
		hour[i] = hour_entry;
		minute[i] = minute_entry;

		// Calculate absolute time to sort entries with
		second[i] = (day[i]-31)*86400 + hour[i]*3600 + minute[i]*60;

		// Replace missing entries with an arbitrary numerical value
		c_score[i] = error_check(c_entry);
		fof2[i] = error_check(fof2_entry);
		fof1[i] = error_check(fof1_entry);	
		foe[i] = error_check(foe_entry);
		foes[i] = error_check(foes_entry);
		hes[i] = error_check(hes_entry);
		hmf2[i] = error_check(hmf2_entry);
		hmf1[i] = error_check(hmf1_entry);
		hme[i] = error_check(hme_entry);
		b0[i] = error_check(b0_entry);
		b1[i] = error_check(b1_entry);

	}
	fclose(AU930);


	// Sort data by ascending time/date
	for(int i = 0; i<line_count-1; i++){		
		sec_min = i;
		for(int j = i+1; j<line_count;j++){
			if(second[j] < second[sec_min]){
				sec_min= j;
			}
		}
		if(sec_min !=i){

			int_swap = second[i];
			second[i] = second[sec_min];
			second[sec_min] = int_swap; 
			
			int_swap = year[i];
			year[i] = year[sec_min];
			year[sec_min] = int_swap;

			int_swap = month[i];
			month[i] = month[sec_min];
			month[sec_min] = int_swap;

			int_swap = date[i];
			date[i] = date[sec_min];
			date[sec_min] = int_swap;
			
			int_swap = day[i];
			day[i] = day[sec_min];
			day[sec_min] = int_swap;

			int_swap = hour[i];
			hour[i] = hour[sec_min];
			hour[sec_min] = int_swap;

			int_swap = minute[i];
			minute[i] = minute[sec_min];
			minute[sec_min] = int_swap;	

			swap = c_score[i];
			c_score[i] = c_score[sec_min];
			c_score[sec_min] = swap;

			swap = fof2[i];
			fof2[i] = fof2[sec_min];
			fof2[sec_min] = swap;

			swap = fof1[i];
			fof1[i] = fof1[sec_min];
			fof1[sec_min] = swap;

			swap = foe[i];
			foe[i] = foe[sec_min];
			foe[sec_min] = swap;

			swap = hes[i];
			hes[i] = hes[sec_min];
			hes[sec_min] = swap;

			swap = hmf2[i];
			hmf2[i] = hmf2[sec_min];
			hmf2[sec_min] = swap;

			swap = hmf1[i];
			hmf1[i] = hmf1[sec_min];
			hmf1[sec_min] = swap;

			swap = hme[i];
			hme[i] = hme[sec_min];
			hme[sec_min] = swap;

			swap = b0[i];
			b0[i] = b0[sec_min];
			b0[sec_min] = swap;

			swap = b1[i];
			b1[i] = b1[sec_min];
			b1[sec_min] = swap;
		}
	}


	// Run data through median filtering function and swap original data with filtered output
	for(int i = 1; i<line_count-1; i++){
		fof2_median_sorter[i] = median_filter(fof2[i-1],fof2[i],fof2[i+1]);
		hmf2_median_sorter[i] = median_filter(hmf2[i-1],hmf2[i],hmf2[i+1]);
	}
	for(int i = 1; i<line_count-1; i++){
		fof2[i] = fof2_median_sorter[i];
		hmf2[i] = hmf2_median_sorter[i];
	}
	
	// Write the sorted/median filtered data to a new text file
	AU930_filtered = fopen("AU930_filtered.txt","w+");
	fprintf(AU930_filtered,\
		"yyyy.MM.dd DD hh:mm:ss csc   fof2   fof1   foe    foes   hes      hmf2     hmf1     hme       bo     b1 \n");
	
	for(int i = 0; i<line_count; i++){
		fprintf(AU930_filtered,\
		"%d.0%d.%2d %d %2d:%2d:00 %.0f %6.3f %6.3f %6.3f %6.3f %7.3f %8.3f %8.3f %8.3f %8.3f %6.3f\n",\
	   		year[i],month[i],date[i],day[i],hour[i],minute[i],\
		 	c_score[i],fof2[i],fof1[i],foe[i],foes[i],hes[i],hmf2[i],hmf1[i],hme[i],b0[i],b1[i]);
	}
	fclose(AU930_filtered);

	// Confirm Successful Run
	printf("File sorted and filtered\n");

	return 0;
}