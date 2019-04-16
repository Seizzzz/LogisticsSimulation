#define INF 0x7fffffff
int x[20],y[20],f[20][35000];
inline void Cal_Distance(struct Point A,struct Point B)

void Cal_ShortestDistance(struct Point Start,struct Point* Map,int Quantity)
{
	f...INF
	for(int s=1;s<=(1<<n)-1;s++)
	{
		for(int i=1;i<=Quantity;i++)
		{
			if((s&(1<<(i-1)))==0) continue;
	        if(s==(1<<(i-1))) {f[i][s]=0;continue;}
	        for(int j=1;j<=n;j++)
	        {
	            if((s&(1<<(j-1)))==0||i==j) continue;
	            f[i][s]=min(f[i][s],f[j][s-(1<<(i-1))]+Cal_Distance(i,j));
	        }
		}
	}
	
	int ans=-1;
	for(int i=1;i<=n;i++)
    {
        int s=f[i][(1<<n)-1]+Cal_Distance(i,0);
        if(ans==-1||ans>s) ans=s;
    }
    
    return ans;
}
