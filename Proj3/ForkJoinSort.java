import java.util.concurrent.*;
import java.util.Random;

class QuickSort extends RecursiveAction
{
    private int left;
    private int right;
    private int[] array;

    public QuickSort(int array[],int left, int right) {
        this.left = left;
        this.right = right;
        this.array = array;
    }
    protected void insertSort(){
//	System.out.print("insertion sort!,right="+(right-left)+"\n");
	for(int i=left+1;i<right+1;i++){
	    int tmp=array[i];
	    int j=i-1;
	    while(j>=left&&array[j]>tmp){
		array[j+1]=array[j];
		j--;
	    }
	    array[j+1]=tmp;
	}
    }
  
    protected int divide(int left,int right){
	    int pivot=array[left];
	    do{
		while(left<right&&array[right]>=pivot)--right;
		if(left<right){
		    array[left]=array[right];
		    ++left;
		}
		while(left<right&&array[left]<=pivot)++left;
		if(left<right){
		    array[right]=array[left];
		    --right;
		}
	    }while(left!=right);
	    array[left]=pivot;
	    return left;
    }

    protected void compute() {
        if(right-left>10){
	    int mid=divide(left,right);
	    RecursiveAction leftpart=new QuickSort(array,left,mid-1);
	    RecursiveAction rightpart=new QuickSort(array,mid+1,right);
	    leftpart.fork();
	    rightpart.fork();
	    leftpart.join();
	    leftpart.join();
	    invokeAll(leftpart,rightpart);
	}else{
	    insertSort();
	}
    }
}


class MergeSort extends RecursiveAction
{
    private int left;
    private int right;
    private int[] array;

    public MergeSort( int[] array,int left, int right) {
        this.left = left;
        this.right = right;
        this.array = array;
    }
    protected void insertSort(){
//	System.out.print("insertion sort!,right="+(right-left)+"\n");
	for(int i=left+1;i<right+1;i++){
	    int tmp=array[i];
	    int j=i-1;
	    while(j>=left&&array[j]>tmp){
		array[j+1]=array[j];
		j--;
	    }
	    array[j+1]=tmp;
	}
    }
    protected void Merge(int left,int mid,int right){
	int len=right-left+1;
	int tmp[]=new int[right-left+1];
	int i=left,j=mid+1,k=0;
	while(i<=mid&&j<=right){
	    tmp[k++]=array[i]<=array[j]?array[i++]:array[j++];
	}
	while(i<=mid){
	    tmp[k++]=array[i++];
	}
	while(j<=right){
	    tmp[k++]=array[j++];
	}
	for(int t=0;t<len;t++){
	    array[left++]=tmp[t];
	}
    }

    protected void compute() {
        if(right-left>10){
	    int mid=left+(right-left)/2;
	    RecursiveAction leftpart=new MergeSort(array,left,mid);
	    RecursiveAction rightpart=new MergeSort(array,mid+1,right);
	    leftpart.fork();
	    rightpart.fork();
	    leftpart.join();
	    leftpart.join();
	    invokeAll(leftpart,rightpart);

	    Merge(left,mid,right);
	}else{
	    insertSort();
	}
    }

}

public class ForkJoinSort {

	static final int SIZE=100;
	
   	private int[] array;
	
	public static void main(String[] args) {
		ForkJoinPool pool = new ForkJoinPool();
		int[] array = new int[SIZE];

		// create SIZE random integers 
		java.util.Random rand = new java.util.Random();

		for (int i = 0; i < SIZE; i++) {
			array[i] = rand.nextInt(1000);
		}
		
		System.out.print("QuickSort(original array):\n");
        	for (int i=0; i<array.length; i++){ 
			System.out.printf("%d ", array[i]);
		}
		System.out.print("\n\n");

		QuickSort quicksort=new QuickSort(array,0,array.length-1);
		pool.invoke(quicksort);
		
		System.out.print("QuickSort(sorted array):\n");
        	for (int i=0; i<array.length; i++){ 
			System.out.printf("%d ", array[i]);
		}
		System.out.print("\n\n\n");



		for (int i = 0; i < SIZE; i++) {
			array[i] = rand.nextInt(100);
		}
		
		System.out.print("MergeSort(original array):\n");
        	for (int i=0; i<array.length; i++){ 
			System.out.printf("%d ", array[i]);
		}
		System.out.print("\n\n");

		MergeSort mergesort=new MergeSort(array,0,array.length-1);
		pool.invoke(mergesort);
	
		System.out.print("MergeSort(sorted array):\n");
       		for (int i=0; i<array.length; i++) 
			System.out.printf("%d ", array[i]);
		System.out.print("\n");	
	}
}
