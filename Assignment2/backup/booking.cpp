#include <bits/stdc++.h>
#define pasid first.first.first
#define trano first.first.second
#define cotype first.second.first
#define numberth first.second.second
#define pref second.first
#define ages second.second
#define mp make_pair
#define pb push_back
#define bpair pair<pair<pair<int,int>,pair<string,int> >,pair<string,string> >
using namespace std;
int train[3][13][72];
vector<pair<int,int> > booked;
int noempty(int a[])
{
	int no=0;
	for(int i=0;i<72;i++)
		if(a[i]==0)
			no++;
	return no;
}
int isther(int i,int seats,int coach[])
{
	
	int ind=0;
	if(seats==72)
	{
		if(i==0)
			ind=2;
		else if(i==1)
			ind=0;
		else if(i==4)
			ind=1;
		else if(i==3)
			ind=7;
		else if(i==2)
			ind=6;
		int s=0;
		while(s<72)
		{
			if(coach[s+ind]==0)
			{
				return (s+ind);
			}
			else if(coach[s+ind+3]==0)
			{
				return (s+ind+3);
			}
			s+=8;
		}
	}
	else
	{
		if(i==0)
			ind=1;
		else if(i==1)
			ind=0;
		else if(i==4)
			ind=1;
		else if(i==3)
			ind=5;
		else if(i==2)
			ind=4;
		int s=0;
		while(s<54)
		{
			if(coach[s+ind]==0)
			{
				return (s+ind);
			}
			else if(coach[s+ind+2]==0)
			{
				return (s+ind+2);
			}
			s+=6;
		}
	}
	return -1;
}
int traincheck(int trainn[][72],int coaches,int seats,int pr[],int total,int ttype)
{
	vector <pair<int,int> > modified;
	for(int i=0;i<5;i++)
	{
		int j=0;
			while(pr[i]>0 && j<coaches)
			{
				int c = isther(i,seats,trainn[j]);
				if(c!=-1)
				{
					pr[i]--;
					train[ttype][j][c]=1;
					modified.pb(mp(j,c));
				}
				else
				{
					j++;
				}
			}
			if(pr[i]!=0)
			{
				for(int p=0;p<modified.size();p++)
					train[ttype][modified[i].first][modified[i].second]=0;
				return -1;
			}
	}
	for(int i=0;i<modified.size();i++)
		booked.pb(modified[i]);
	return 1;
}
int continuousbooking(int trainn[][72],int coaches,int seats,int total)
{
	for(int i=0;i<coaches;i++)
	{
		for(int j=0;j<seats-total+1;j++)
		{
			bool av=true;
			for(int k=j;k<j+total;k++)
			{
				if(trainn[i][k]==1)
				{
					av=false;
					break;
				}
			}
			if(av==true)
			{
				for(int p=0;p<total;p++)
					booked.pb(mp(i,j+p));
				return 1;
			}
		}
	}
	return -1;
}
int randombooking(int trainn[][72],int coaches,int seats,int total)
{
	vector<pair<int,int> > tmp;
	for(int i=0;i<coaches;i++)
	{
		for(int j=0;j<seats;j++)
		{
			if(train[i][j]==0)
				tmp.pb(mp(i,j));
			if(tmp.size()==total)
			{
				for(int i=0;i<total;i++)
					booked.pb(tmp[i]);
				return 1;
			}
		}
	}
	return -1;
}
int* coachcheck(int coach[],int seats,int pr[],int total)
{
	int *allotted=(int*)calloc(total,sizeof(int));
	int indarr[total];
	int tmp[5];
	int aind=0;
	for(int i=0;i<5;i++)
		tmp[i]=pr[i];
	if(seats==72)
	{
		for(int i=0;i<5;i++)
		{
			bool side=false;
			int s=0;
			int ind=0;
			if(i==0)
				ind=2;
			else if(i==1)
				ind=0;
			else if(i==4)
				ind=1;
			else if(i==3)
			{
				side=true;
				ind=7;
			}
			else if(i==2)
			{
				side=true;
				ind=6;
			}

			while(tmp[i]>0 && s<72)
			{
				if(coach[s+ind]==0)
				{
					allotted[aind++]=s+ind;
					tmp[i]--;
				}
				if(tmp[i]==0 || side==true)
				{
					s+=8;
					continue;
				}
				if(coach[s+3+ind]==0)
				{
					allotted[aind++]=s+3+ind;
					tmp[i]--;
				}
				s+=8;
			}

		}
	}
	else
	{
		for(int i=0;i<4;i++)
		{
			bool side=false;
			int s=0;
			int ind=0;
			if(i==0)
				ind=1;
			else if(i==1)
				ind=0;
			else if(i==3)
			{
				side=true;
				ind=5;
			}
			else if(i==2)
			{
				side=true;
				ind=4;
			}

			while(tmp[i]>0 && s<54)
			{
				if(coach[s+ind]==0)
				{
					allotted[aind++]=s+ind;
					tmp[i]--;
				}
				if(tmp[i]==0 || side==true)
				{
					s+=6;
					continue;
				}
				if(coach[s+3+ind]==0)
				{
					allotted[aind++]=s+3+ind;
					tmp[i]--;
				}
				s+=6;
			}
		}
	}
	if(aind<total)
	{
		allotted[0]=-1;
	}
	else
	{
		for(int i=0;i<total;i++)
			coach[allotted[i]]=1;
	}
	return allotted;
}
int booking(bpair book)
{
	int ttype,coaches,seats;
	if(book.trano==12301)
	{
		ttype=0;
		coaches=13;
		seats=54;
	}
	else if(book.trano==12321)
	{
		if(book.cotype=="AC")
		{
			ttype=1;
			coaches=3;
		}
		else
		{
			ttype=2;
			coaches=12;
		}
		seats=72;
	}
	else
	{
		printf("No such train found\n");
		return -1;
	}

	istringstream iss(book.pref);
	std::size_t found;
	std::string token;
  	found = token.find("NA");
  	bool nopref=false;
  	int bpr[5]; // up,lb,sl,su,mb;
	if(found!=std::string::npos)
	{
		nopref=true;
	}
	else
	{
		vector<string> preff;
		memset(bpr,0,sizeof(bpr));
		while (std::getline(iss, token, '-'))
		{	
	  		found = token.find("UB");
		    if(found!=std::string::npos)
		    	bpr[0]++;
		    found = token.find("LB");
		    if(found!=std::string::npos)
		    	bpr[1]++;
		    found = token.find("SL");
		    if(found!=std::string::npos)
		    	bpr[2]++;
		    found = token.find("SU");
		    if(found!=std::string::npos)
		    	bpr[3]++;
		    found = token.find("MB");
		    if(found!=std::string::npos)
		    	bpr[4]++;
		}
	}
	int tcheck;
	if(!nopref)
	{
		int acoach=-1;
		for(int i=0;i<coaches;i++)
		{
			int *tickets = coachcheck(train[ttype][i],seats,bpr,book.numberth);
			if(tickets[0]!=-1)
			{
				acoach=i+1;
				for(int j=0;j<book.numberth;j++)
					booked.pb(mp(i,tickets[j]));
				return 1;
			}
		}
		tcheck=traincheck(train[ttype],coaches,seats,bpr,book.numberth,ttype);
		if(tcheck)
			return 1;
	}
	tcheck=continuousbooking(train[ttype],coaches,seats,book.numberth);
	if(tcheck)
		return 1;
	tcheck=randombooking(train[ttype],coaches,seats,book.numberth);
	if(tcheck)
		return 1;
	return -1;
}
int main()
{
	memset(train,0,sizeof(train));
	bpair temp;
	ifstream file("Booking.csv"); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	string line;
	while(getline(file, line)) 
	{
	    std::size_t prev = 0, pos;
	    int no=0;
	    while ((pos = line.find_first_of(" ',", prev)) != std::string::npos)
	    {
	        if (pos > prev)
	        {
	        	if(no==0)
	        		temp.pasid=atoi(line.substr(prev, pos-prev).c_str());
	        	else if(no==1)
	        		temp.trano=atoi(line.substr(prev, pos-prev).c_str());
	        	else if(no==2)
	        		temp.cotype=line.substr(prev, pos-prev);
	        	else if(no==3)
	        		temp.numberth=atoi(line.substr(prev, pos-prev).c_str());
	        	else if(no==4)
	        		temp.pref=line.substr(prev, pos-prev);
	            no++;
	        }
	        prev = pos+1;
	    }
	    if (prev < line.length())
	        temp.ages=line.substr(prev, std::string::npos);
	    break;
	}
	cout<<temp.pasid<<" . "<<temp.trano<<" . "<<temp.cotype<<" . "<<temp.numberth<<" . "<<temp.pref<<" . "<<temp.ages<<"\n";
	booking(temp);
	for(int i=0;i<booked.size();i++)
		printf("coach-> %d  ticket-> %d\n",booked[i].first,booked[i].second);
	booked.clear();
	booking(temp);
	for(int i=0;i<booked.size();i++)
		printf("coach-> %d  ticket-> %d\n",booked[i].first,booked[i].second);
	booked.clear();
	return 0;
}