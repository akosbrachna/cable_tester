void sort(unsigned int data[], unsigned char elemszam)
{
int i, j, temp;

    for(i = 0; i < elemszam; i++)
    {
        for(j = i; j < elemszam; j++)
        {
            if(data[i] > data[j])
            {
                temp    = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

void stat(unsigned int data[], unsigned char elemszam, unsigned char minmax_index[])
{
unsigned int min_mid, max_mid;
unsigned char i;

	min_mid = data[5]*9;
	max_mid = data[5]*11;

	minmax_index[0] = 0;
 	for (i = 0; i < elemszam; i++)
	{
		if ((data[i]*10) < min_mid)
		{
			minmax_index[0] = i+1;
		} 
		else break;
	}

	minmax_index[1] = elemszam-1;
	for (i = 0; i < elemszam; i++)
	{
		if ((data[i]*10) > max_mid)
		{	
			minmax_index[1] = i;
			break;
		}
	}
}

unsigned int average(unsigned int data[], unsigned char minmax_index[])
{
unsigned char i = 0;
unsigned int sum = 0;

	for (i = minmax_index[0]; i < minmax_index[1]; i++)
	{
		sum += data[i];
	}
	return sum/(minmax_index[1]-minmax_index[0]);
}
