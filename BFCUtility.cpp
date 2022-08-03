#include <BFCUtility.h>





//************************************************************************
//
//						BFCVoidNode
//
//************************************************************************



//************************************************************************
//
//						BFCVoidList
//
//************************************************************************





//*****************************************************************************************************
//
//
//
//								Global Utility Function
//
//
//
//
//*****************************************************************************************************



BFCChar *Float2Char(BFCFloat f)
{
	static BFCChar buff[256];
	static BFCChar buff2[256];

	BFCInt sign = 0;
	BFCInt dec = 0;
	BFCInt i = 0;
	BFCInt j = 0;
	BFCChar *pchar;
	BFCChar tmp_char;
	BFCChar save;

	if ( fabs(f) < 0.0000005f)
	{
		strcpy(buff,"0.0000000");
		return buff;
	}
	pchar = _fcvt(f,7,&dec,&sign);
	// put the '.' at the right location
	if (dec >= 0)
	{
		i = dec;
		tmp_char = pchar[i];
		pchar[i] = '.'; 
		do
		{	
			i++;
			save = pchar[i];
			pchar[i] = tmp_char;
			tmp_char = save;
		} while (pchar[i] != '\0');
		strcpy(buff2,pchar);
	} else 
	{
		i=0;
		buff2[i] = '0';
		i++;
		buff2[i] = '.';
		j=dec;
		i++;
		while (j != 0)
		{
			buff2[i] = '0';
			j++;
			i++;
		}
		buff2[i] = '\0';
		strcat(buff2,pchar);
	}
	if (dec==0)
	{
		buff2[0] = '0';
		buff2[1] = '\0';
		strcat(buff2,pchar);
	}

	if (sign!=0)
	{
		buff[0] = '-';
		buff[1] = '\0';
		strcat(buff,buff2);
		return buff;
	} else
	{
		return buff2;
	}
}
