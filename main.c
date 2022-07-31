#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct hf{
    int freq;
    int parent;
    int lchild;
    int rchild;
}HuffmanTree;

typedef struct hc{
    int* bit;
    int start;
    char ch;
}HuffmanCode;


#define SIZE 100000

struct DataItem {
   char data[20];   
   char key;
};

struct DataItem* hashArray[SIZE]; 
struct DataItem* dummyItem;
struct DataItem* item;

int hasHuffmanCode(char key) {
   return key % SIZE;
}

struct DataItem *search(char key) {
   int hashIndex = hasHuffmanCode(key);  
	
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex]; 
			
      ++hashIndex;
		
      hashIndex %= SIZE;
   }        
	
   return NULL;        
}

void insert(int key, char data[]) {

   struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
   strcpy(item->data, data); 
   item->key = key;

   int hashIndex = hasHuffmanCode(key);

   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
      ++hashIndex;
		
      hashIndex %= SIZE;
   }
	
   hashArray[hashIndex] = item;
}


HuffmanTree* InitHuffmanTree(int* frequency,int n){
    HuffmanTree* HT = (HuffmanTree*)malloc(sizeof(HuffmanTree)*(2*n-1));
    for(int i = 0; i<n; i++){
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
        HT[i].freq = frequency[i];
        }
        return HT;
}

HuffmanCode* InitHuffmanCode(int n,char* ch){
    HuffmanCode* HC = (HuffmanCode*)malloc(n*sizeof(HuffmanCode));

    for(int i = 0; i<n; i++){
        HC[i].bit = (int*)malloc(n*sizeof(int));
        HC[i].ch = ch[i];
    }
    
    return HC;
}


void BuildHuffmanTree(HuffmanTree* HT,int n){
    int i,j,a,b,lowest,sec_lowest;
    int l = 2*n-1;
    for(i = n;i<l;i++){
        a = 100000;
        b = 100000;
        lowest = 0;
        sec_lowest = 0;
        for(j = 0; j < n+i; j++){
            if(HT[j].parent == -1 && HT[j].freq < a){
                a = HT[j].freq;
                lowest = j;
            }
        }
        for(j = 0; j < n+i; j++){
            if(HT[j].parent == -1 && HT[j].freq < b && j!= lowest){
                b = HT[j].freq;
                sec_lowest = j;
            }
        }
        HT[i].parent = -1;

        HT[i].lchild = lowest;

        HT[i].rchild = sec_lowest;

        HT[i].freq = (HT[lowest].freq)+(HT[sec_lowest].freq);
        
        HT[lowest].parent = i;

        HT[sec_lowest].parent = i;
        n++;

    }

}
		
void BuildHuffCodes(HuffmanCode* HC,HuffmanTree* HT,int n){
    int parent, temp;

    for(int i = 0; i<n;i++){
        HuffmanCode* huffc = (HuffmanCode*)malloc(sizeof(HuffmanCode));
        huffc->bit = (int*)malloc(sizeof(int)*n);
        huffc->start = n-1;
        temp = i;
        parent = HT[temp].parent;

        while(parent!=-1){

            if(HT[parent].rchild == temp){
                huffc->bit[huffc->start] = 1;
            }else{
                huffc->bit[huffc->start] = 0;
            }
            huffc->start--;
            temp = parent;
            parent = HT[temp].parent;

        }
        for(int j = huffc->start+1; j < n; j++){
            HC[i].bit[j] = huffc->bit[j];

        }
        
        HC[i].start = huffc->start;
        
        free(huffc);
    }
}			
void PrintCodes(HuffmanCode* HC,int n){

	for(int i = 0; i < n; i++){
        char res[20] = {'\0'};
        for(int j = HC[i].start+1; j < n; j++){
            char bit[2];
            sprintf(bit,"%d",HC[i].bit[j]);
			strcat(res, bit);
        }
        insert(HC[i].ch, res);
        printf("%c -> %s\n", HC[i].ch, res);
	}

}

int LinSearch(char arr[], char x, int n) {
    int isElementPresent = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] == x) {
            isElementPresent = 1;
            break;
        }
    }
    return isElementPresent;
}

void Encode(char inp_file[], char out_file[], FILE *fp) {
    char string[1000000];
    fp = fopen(inp_file, "r");
    fscanf(fp, "%s", string);
    fclose(fp);
    char arr[1000000];
    int freq[1000000];
    int idx=0;
    for(int i=0; i<strlen(string); i++) {
        if (LinSearch(arr, string[i], idx)==1) {
            continue;
        }else {
            arr[idx] = string[i];
            idx++;
        }
    }
    // printf("%d\n", idx);
    for(int i=0; i<idx; i++) {
        // printf("%c\n", arr[i]);
        freq[i] = 0;
        for(int j=0; j<strlen(string); j++) {
            if (arr[i]==string[j]) {
                freq[i]++;
            }
        }
    }

    HuffmanTree * HT = InitHuffmanTree(freq,idx);
    HuffmanCode * HC = InitHuffmanCode(idx,arr);

    BuildHuffmanTree(HT,idx);

    BuildHuffCodes(HC,HT,idx);

    PrintCodes(HC,idx);
    char encodedstring[1000000] = {'\0'};
    // static char* encodedstring[1000000] = {'\0'};
    for(int i=0; i<strlen(string); i++) {
        item = search(string[i]);
        strcat(encodedstring, item->data);
    }
    fp = fopen(out_file, "w");
    // char uniq_chars[1000000]={'\0'};
    for(int i=0; i<idx; i++) {
        fprintf(fp, "%c_", arr[i]);
    }
    fprintf(fp, "\n");
    for(int i=0; i<idx; i++) {
        fprintf(fp, "%d_", freq[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%s", encodedstring);
    fclose(fp);
    // return encodedstring;
}

void Decode(char inp_file[], char out_file[], FILE *fp){
    char uniq_chars[1000000];
    char *freq_[10000];
    fp = fopen(inp_file, "r");
    fscanf(fp, "%s\n", uniq_chars);
    // printf("%s\n", uniq_chars);
    char delim[] = "_";
    char *ptr = strtok(uniq_chars, delim);
    char *arr_[10000];
    int idx=0;
    while(ptr != NULL)
	{
		arr_[idx] = ptr;
		ptr = strtok(NULL, delim);
        idx++;
	}
    char arr[10000];
    for(int i=0; i<idx; i++) {
        char val = *arr_[i];
        arr[i] = val;
    }
    // for(int i=0; i<idx; i++) {
    //     printf("%s\n", arr[i]);
    // }
    fscanf(fp, "%s\n", uniq_chars);
    // printf("%s\n", uniq_chars);
    char *ptr1 = strtok(uniq_chars, delim);
    idx=0;
    while(ptr1 != NULL)
	{
        freq_[idx] = ptr1;
		ptr1 = strtok(NULL, delim);
        idx++;
	}
    int freq[10000];
    for(int i=0; i<idx; i++) {
        int val = atoi(freq_[i]);
        freq[i] = val;
    }

    HuffmanTree * HT = InitHuffmanTree(freq,idx);
    HuffmanCode * HC = InitHuffmanCode(idx,arr);

    BuildHuffmanTree(HT,idx);

    BuildHuffCodes(HC,HT,idx);
    char encoded[1000000];
    fscanf(fp, "%s", encoded);
    fclose(fp);
    // printf("%s\n", encoded);
    int n = idx;
    idx=2*n-2;
    int i=0;
    fp = fopen(out_file, "w");
    while(encoded[i]!='\0'){
        if(encoded[i] != '0'){
            idx = HT[idx].rchild;
        }else{
            idx = HT[idx].lchild;
        }
        if((HT[idx].rchild == -1) && (HT[idx].lchild == -1)){          
                fprintf(fp, "%c", HC[idx].ch);
                idx = 2*n-2;  
        }
        i++;
    }
    fclose(fp);
}


int main(int argc, char const *argv[])
{
    FILE *fp = NULL;
    char in_file[100];
    char out_file[100];
    printf("Enter input file name: ");
    scanf("%s", in_file);
    printf("Enter output file name: ");
    scanf("%s", out_file);
    printf("Enter mode (0 for compression, 1 for decompression):");
    int mode;
    scanf("%d", &mode);
    // scanf("%s", string);
    if(mode == 0) {
        Encode(in_file, out_file, fp);
    }else {
        Decode(in_file, out_file, fp);
    }
    return 0;
}