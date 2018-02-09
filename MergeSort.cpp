typedef struct {
	unsigned long id;          // Node identification
	char name[184];
	double lat, lon;           // Node position
	unsigned short nsucc;      // Number of node successors; i. e. length of successors
	unsigned long *successors;
} node;


void swap(node *a, node *b)
{
    unsigned long id = a->id;
	double lon=a->lon;
	double lat=a->lat;

    a->id = b->id;
    a->lon = b->lon;
    a->lat = b->lat;
	b->id = id;
	b->lon = lon;
	b->lat = lat;
}

int partition (node arr[], int low, int high, int pivotN)
{
    int pivot = arr[pivotN].id;    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j].id <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(node arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high, 5);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
