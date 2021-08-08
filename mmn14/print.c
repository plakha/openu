int main()
{

	    unsigned long data = 0x001;
	        unsigned long *pdata = &data;
		    
		    unsigned char *pbyte = (unsigned char *)pdata;
		        
		        
		        for (int i = 0; i < 4; ++i)
				    {
					            printf(" %02X", *(pbyte + i));
						        }

			    return 0;
}
