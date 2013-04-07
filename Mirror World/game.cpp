/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Transformations and Timers" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */


#include<cstdio>
#include <iostream>
#include <stdlib.h>
#include<math.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
#define PI 3.14159265

//Called when a key is pressed

//Initializes 3D rendering
void initRendering() {
	glEnable(GL_DEPTH_TEST);
}

//Called when the window is resized


float _angle = 30.0f;
float _cameraAngle = 0.0f;
float w;
float h;
int pn;
float project_x1[20];
float project_x2[20];
float project_y1[20];
float project_y2[20];
float project_d[20];
int project_n[20];
int block_n;
float block_x1[20];
float block_x2[20];
float block_y1[20];
float block_y2[20];
float block_slope[20];
int mirror_n;
float mirror_x1[20];
float mirror_x2[20];
float mirror_y1[20];
float mirror_y2[20];
float mirror_slope[20];
float intersect1,intersect2;
int keycount=-1;
int total=0;
float trgap=0.3;
int uniflag=-1;
int rdiff=0;
int ldiff=0;
float rotangle=0.1;
int stripflag=-1;
int animation_mode=-1;
int colorflag=0;
float rc[50];
float gc[50];
float bc[50];
float zoom=-120.0f;
float insect_x[20];
float insect_y[20];
double posX,posY,posZ;
int gaze=-1;

void handleResize(int w1, int h1) {

	glViewport(0, 0, w1, h1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(-w/2,w/2,-h/2,h/2);
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawLine(float x1,float x2,float y1,float y2){
	//glScalef(0.9,0.9,0.0);
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
}
/*
   void drawLine3(float x1,float y1){
//glScalef(0.9,0.9,0.0);
glLineWidth(2.5f);
glBegin(GL_LINES);
glVertex2f(x1,y1);
glVertex2f(x1,y1);
glEnd();
glLineWidth(1.0f);
}
*/

void drawLine2(float x1,float x2,float y1,float y2,int obj){
	glPushMatrix();
	glLineWidth(2.5f);
	if(obj==0)
	{
		glLineWidth(3.5f);
		glColor3f(0.9f, 0.6f, 0.5f);	
	}
	else if(obj==1)
		glColor3f(0.2f, 0.4f, 0.7f);	
	else{
		glColor3f(0.8f, 0.8f, 0.8f);
	}
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();


}
void highlight(float x1,float x2,float y1,float y2){
	glPushMatrix();
	glLineWidth(2.0f);
	glColor3f((rand()%10)/10.0,(rand()%10)/10.0,(rand()%10)/10.0);
	glBegin(GL_LINES);
	glVertex2f(x1+0.1,y1+0.1);
	glVertex2f(x2+0.1,y2+0.1);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x1-0.1,y1-0.1);
	glVertex2f(x2-0.1,y2-0.1);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x1+0.2,y1+0.2);
	glVertex2f(x2+0.2,y2+0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x1-0.2,y1-0.2);
	glVertex2f(x2-0.2,y2-0.2);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void drawLine1(float x1,float x2,float y1,float y2,float slope,float r,float g,float b){
	glPushAttrib(GL_ENABLE_BIT); 
	if(animation_mode==1)
	{
		if(stripflag==-1)
			glLineStipple(0.2, 0xF0F0);	
		else
			glLineStipple(0.2, 0x0F0F);	
		glEnable(GL_LINE_STIPPLE);
	}
	glColor3f(r,g,b);	
	drawLine(x1,x2,y1,y2);
	glPopAttrib();

}



void drawPoint(float a,float b){
	glBegin(GL_POINTS);
	glVertex2f(a,b);
	glEnd();
}


void drawholes(float prx[],float pry[],int n){
	int i;
	glColor3f(1.0f, 0.7f, 0.0f);
	for(i=0;i<n;i++)
	{
		drawPoint(prx[i],pry[i]+0.3f);
	}	
}
int chk_block(float project_slope,float project_x,float project_y,float endx[],float endy[],int i,float intersect1,float intersect2){
	int j;
	float mind=9999;
	for(j=0;j<block_n;j++){
		if(project_slope==block_slope[j])
			drawLine(project_x,endx[i],project_y,endy[i]);
		else{
			float x1=project_x;
			float y1=project_y;
			float x2=endx[i];
			float y2=endy[i];
			float x3=block_x1[j];
			float y3=block_y1[j];
			float x4=block_x2[j];
			float y4=block_y2[j];
			float p1,p2;
			float denom=(x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
			p1=(((x1*y2-y1*x2)*(x3-x4))-((x1-x2)*(x3*y4-y3*x4)))/denom;
			p2=(((x1*y2-y1*x2)*(y3-y4))-((y1-y2)*(x3*y4-y3*x4)))/denom;
			float d1=sqrt((x3-p1)*(x3-p1)+(y3-p2)*(y3-p2));
			float d2=sqrt((x4-p1)*(x4-p1)+(y4-p2)*(y4-p2));
			float d3=sqrt((x4-x3)*(x4-x3)+(y4-y3)*(y4-y3));
			//cout<<d1<<" "<<d2<<" "<<d3<<endl;
			if(((d1+d2)<=d3+1)&&((d1+d2)>=d3-1))
			{
				float id1=sqrt((x1-intersect1)*(x1-intersect1)+(y1-intersect2)*(y1-intersect2));
				float id2=sqrt((intersect1-p1)*(intersect1-p1)+(intersect2-p2)*(intersect2-p2));
				float id3=sqrt((x1-p1)*(x1-p1)+(y1-p2)*(y1-p2));
				if((int)(id1+id2)!=(int)id3)
				{
					if(id3<=mind)
					{
						mind=id3;
						endx[i]=p1;
						endy[i]=p2;
					}
				}
			}

		}
	}
	return mind;
}

void check(float x1,float y1,float iniendx,float iniendy,float slope,float r,float g,float b){
	//	drawLine(x1,iniendx,y1,iniendy);
	int flag=0,diff_flag=0;
	int mind=9999,min1=9999;
	float minendx,minendy,minslope,minrendx1,minrendy1,minrendx2,minrendy2;
	float endx1,endy1;
	//cout<<slope<<endl;
	int i,j;		
	for(j=0;j<block_n;j++){
		if(slope==block_slope[j]){}
		//drawLine(x1,endx1,y1,endy1);
		else{
			float x2=iniendx;
			float y2=iniendy;
			float x3=block_x1[j];
			float y3=block_y1[j];
			float x4=block_x2[j];
			float y4=block_y2[j];
			float p1,p2;
			float denom=(x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
			p1=(((x1*y2-y1*x2)*(x3-x4))-((x1-x2)*(x3*y4-y3*x4)))/denom;
			p2=(((x1*y2-y1*x2)*(y3-y4))-((y1-y2)*(x3*y4-y3*x4)))/denom;
			if((int)p1!=(int)x1 && (int)p2!=(int)y1)
			{
				//cout<<"p1= "<<p1<<endl<<" p2= "<<p2<<endl;
				float d1=sqrt((x3-p1)*(x3-p1)+(y3-p2)*(y3-p2));
				float d2=sqrt((x4-p1)*(x4-p1)+(y4-p2)*(y4-p2));
				float d3=sqrt((x4-x3)*(x4-x3)+(y4-y3)*(y4-y3));
				//cout<<d1<<" "<<d2<<" "<<d3<<endl;
				if(((d1+d2)<=d3+1)&&((d1+d2)>=d3-1))
				{
					float bd1=sqrt((x1-p1)*(x1-p1)+(y1-p2)*(y1-p2));
					float bd2=sqrt((iniendx-p1)*(iniendx-p1)+(iniendy-p2)*(iniendy-p2));
					float bd3=sqrt((x1-iniendx)*(x1-iniendx)+(y1-iniendy)*(y1-iniendy));
					if(((bd1+bd2)<=bd3+1)&&((bd1+bd2)>=bd3-1))
					{
						//cout<<slope<<endl;
						flag=1;
						if(bd1<mind){
							mind=bd1;
							//cout<<"hi"<<endl;
							endx1=p1;
							endy1=p2;
							minendx=p1;
							minendy=p2;
							//cout<<"p1= "<<p1<<" p2= "<<p2;
							//drawLine1(x1,endx1,y1,endy1,slope);
						}
					}
				}
			}


		}
	}


	for(j=0;j<mirror_n;j++){
		if(slope==mirror_slope[j]){}
		//drawLine(x1,endx1,y1,endy1);
		else{
			float ax1=x1;
			float ay1=y1;
			float ax2=iniendx;
			float ay2=iniendy;
			float ax3=mirror_x1[j];
			float ay3=mirror_y1[j];
			float ax4=mirror_x2[j];
			float ay4=mirror_y2[j];
			float ap1,ap2;
			float adenom=(ax1-ax2)*(ay3-ay4)-(ay1-ay2)*(ax3-ax4);
			ap1=(((ax1*ay2-ay1*ax2)*(ax3-ax4))-((ax1-ax2)*(ax3*ay4-ay3*ax4)))/adenom;
			ap2=(((ax1*ay2-ay1*ax2)*(ay3-ay4))-((ay1-ay2)*(ax3*ay4-ay3*ax4)))/adenom;
			if((int)ap1!=(int)x1 && (int)ap2!=(int)y1)
			{
				//cout<<ap1<<endl<<ap2<<endl;
				//cout<<"hello   "<<x1<<"   "<<y1<<endl;
				float ad1=sqrt((ax3-ap1)*(ax3-ap1)+(ay3-ap2)*(ay3-ap2));
				float ad2=sqrt((ax4-ap1)*(ax4-ap1)+(ay4-ap2)*(ay4-ap2));
				float ad3=sqrt((ax4-ax3)*(ax4-ax3)+(ay4-ay3)*(ay4-ay3));

				if(((ad1+ad2)<=ad3+1)&&((ad1+ad2)>=ad3-1))
				{
					float cd1=sqrt((x1-ap1)*(x1-ap1)+(y1-ap2)*(y1-ap2));
					float cd2=sqrt((iniendx-ap1)*(iniendx-ap1)+(iniendy-ap2)*(iniendy-ap2));
					float cd3=sqrt((x1-iniendx)*(x1-iniendx)+(y1-iniendy)*(y1-iniendy));
					if(((cd1+cd2)<=cd3+1)&&((cd1+cd2)>=cd3-1))
					{
						if(cd1<min1 && cd1<mind){
							float diffa=ap1-ax1;
							float diffb=ap2-ay1;
							float diffc=ax4-ax3;
							float diffd=ay4-ay3;
							float vector_res=(diffa*diffd)-(diffb*diffc);
							if(vector_res>=0)
								diff_flag=1;
							min1=cd1;
							flag=2;
							endx1=ap1;
							endy1=ap2;
							minrendx1=ap1;
							minrendy1=ap2;
							float m;
							if((ax4-ax3)!=0)
								m=(ay4-ay3)/(ax4-ax3);
							else
								m=(ay4-ay3)/0.1;
							//	float m=(ay4-ay3)/(ax4-ax3);
							float c=ay3-m*ax3;
							if(m==0)
								m=0.1;
							float n=(-1)/(m);
							float d=ay1-n*ax1;
							float mx= (-c+d)/(m-n);
							float my=(d*m-c*n)/(m-n);
							float mirx=2*mx-ax1;
							float miry=2*my-ay1;
							float slope1=(ap2-miry)/(ap1-mirx);
							float anewx1=w/2;
							float anewy1=miry+ slope1*(anewx1-mirx);
							float anewx2=-w/2;
							float anewy2=miry+ slope1*(anewx2-mirx);
							float sign1=(anewy1-ay3)-m*(anewx1-ax3);
							float sign2=(anewy2-ay3)-m*(anewx2-ax3);
							float sign3=(ay1-ay3)-m*(ax1-ax3);
							minslope=slope1;
							//drawLine1(x1,ap1,y1,ap2,slope1);					
							if(sign2*sign3 >0){
								minrendx2=anewx2;
								minrendy2=anewy2;

								//cout<<"ax1="<<ax1<<"   ay1="<<ay1<<endl;
								//drawLine(ap1,anewx2,ap2,anewy2);
								//check(ap1,ap2,anewx2,anewy2,slope1);
							}	
							else{
								minrendx2=anewx1;
								minrendy2=anewy1;
								//cout<<"hello"<<endl;
								//drawLine(ap1,anewx1,ap2,anewy1);
								//check(ap1,ap2,anewx1,anewy1,slope1);
							}
						}}
				}
			}


		}

	}
	if(flag==1){
		//	cout<<x1<<"   "<<minendx<<"  "<<y1<<" "<<minendy<<endl;
		drawLine1(x1,minendx,y1,minendy,slope,r,g,b);
	}
	if(flag==2){
		drawLine1(x1,minrendx1,y1,minrendy1,minslope,r,g,b);					
		if(diff_flag==0)
			check(minrendx1,minrendy1,minrendx2,minrendy2,minslope,r,g,b);
	}
	if(flag==0)
	{
		drawLine1(x1,iniendx,y1,iniendy,slope,r,g,b);
	}

}

void chk_mirror(float project_slope,float project_x,float project_y,float endx[],float endy[],int i,float intersect1,float intersect2,float mi,float r,float g,float b){
	int j;
	int flag3=0;
	int diff_flag=0;
	float mind=mi;
	float m1,m2;
	float startx,starty;
	float minslope;
	for(j=0;j<mirror_n;j++){
		if(project_slope==mirror_slope[j])
			drawLine1(project_x,endx[i],project_y,endy[i],project_slope,r,g,b);
		else{
			float x1=project_x;
			float y1=project_y;
			float x2=endx[i];
			float y2=endy[i];
			float x3=mirror_x1[j];
			float y3=mirror_y1[j];
			float x4=mirror_x2[j];
			float y4=mirror_y2[j];
			float p1,p2;
			float denom=(x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
			p1=(((x1*y2-y1*x2)*(x3-x4))-((x1-x2)*(x3*y4-y3*x4)))/denom;
			p2=(((x1*y2-y1*x2)*(y3-y4))-((y1-y2)*(x3*y4-y3*x4)))/denom;
			float d1=sqrt((x3-p1)*(x3-p1)+(y3-p2)*(y3-p2));
			float d2=sqrt((x4-p1)*(x4-p1)+(y4-p2)*(y4-p2));
			float d3=sqrt((x4-x3)*(x4-x3)+(y4-y3)*(y4-y3));
			//cout<<d1<<" "<<d2<<" "<<d3<<endl;
			if(((d1+d2)<=d3+1)&&((d1+d2)>=d3-1))
			{
				float id1=sqrt((x1-intersect1)*(x1-intersect1)+(y1-intersect2)*(y1-intersect2));
				float id2=sqrt((intersect1-p1)*(intersect1-p1)+(intersect2-p2)*(intersect2-p2));
				float id3=sqrt((x1-p1)*(x1-p1)+(y1-p2)*(y1-p2));
				if((int)(id1+id2)!=(int)id3)
				{

					if(id3<mind)
					{
						float diffa=p1-x1;
						float diffb=p2-y1;
						float diffc=x4-x3;
						float diffd=y4-y3;
						float vector_res=(diffa*diffd)-(diffb*diffc);
						if(vector_res>=0)
							diff_flag=1;
						flag3=1;
						mind=id3;
						endx[i]=p1;
						endy[i]=p2;
						startx=p1;
						starty=p2;
						float m;
						if((x4-x3)!=0)
							m=(y4-y3)/(x4-x3);
						else
							m=(y4-y3)/0.1;
						float c=y3-m*x3;
						if(m==0)
							m=0.1;
						float n=(-1)/(m);
						float d=y1-n*x1;
						float mx= (-c+d)/(m-n);
						float my=(d*m-c*n)/(m-n);
						float mirx=2*mx-x1;
						float miry=2*my-y1;
						float slope=(p2-miry)/(p1-mirx);
						float newx1=w/2;
						float newy1=miry+ slope*(newx1-mirx);
						float newx2=-w/2;
						minslope=slope;
						float newy2=miry+ slope*(newx2-mirx);
						float sign1=(newy1-y3)-m*(newx1-x3);
						float sign2=(newy2-y3)-m*(newx2-x3);
						float sign3=(project_y-y3)-m*(project_x-x3);
						if(sign2*sign3 >=0){
							m1=newx2;
							m2=newy2;
							//check(p1,p2,newx2,newy2,slope);
							//drawLine(p1,newx2,p2,newy2);
						}
						else{
							m1=newx1;
							m2=newy1;
							//check(p1,p2,newx1,newy1,slope);
							//drawLine(p1,newx1,p2,newy1);
						}
					}
				}
			}


		}
	}
	if(flag3==1 && diff_flag==0)
		check(startx,starty,m1,m2,minslope,r,g,b);
}

void drawRays(float project_holes_x[],float project_holes_y[],float center_x,float center_y,int n){
	float endy[n];
	float endx[n];
	float end1y[n];
	float end1x[n];
	int i,j;
	float project_slope[n];
	float mirror_slope[n];
	for(i=0;i<n;i++){
		if(project_holes_x[i]!=center_x){
			project_slope[i]=(project_holes_y[i]-center_y)/(project_holes_x[i]-center_x);
			if(project_holes_x[i]>=center_x){
				endx[i]=w/2;
				end1x[i]=w/2;
				endy[i]=project_holes_y[i]+ project_slope[i]*(endx[i]-project_holes_x[i]);
				end1y[i]=project_holes_y[i]+ project_slope[i]*(endx[i]-project_holes_x[i]);
			}
			else{
				endx[i]=-w/2;
				end1x[i]=-w/2;
				endy[i]=project_holes_y[i]+ project_slope[i]*(endx[i]-project_holes_x[i]);
				end1y[i]=project_holes_y[i]+ project_slope[i]*(endx[i]-project_holes_x[i]);
			}
		}
		else{
			endx[i]=0.0f;
			end1x[i]=200.0f;
			endy[i]=400.0f;
			end1y[i]=project_holes_y[i]+ project_slope[i]*(endx[i]-project_holes_x[i]);
		}
		float mi=chk_block(project_slope[i],project_holes_x[i],project_holes_y[i],endx,endy,i,center_x,center_y);
		chk_mirror(project_slope[i],project_holes_x[i],project_holes_y[i],endx,endy,i,center_x,center_y,mi,rc[i],gc[i],bc[i]);
		drawLine1(project_holes_x[i],endx[i],project_holes_y[i],endy[i],project_slope[i],rc[i],gc[i],bc[i]);

	}
	colorflag=1;

}


void drawProjector(float x1,float x2,float y1,float y2,float d,int n){
	int center_x=x1+(x2-x1)/2;
	int center_y;
	if(y1>0)
		center_y=y1+d;
	else
		center_y=y1-d;

	float m1=(y2-y1)/(x2-x1);
	float deno=sqrt(1+m1*m1);
	float c=y1-m1*x1;
	float d1=d;
	float c1=c-d1*deno;
	if(m1==0)
		m1=0.1;
	float m2=-1/(m1);
	float c2=(y1+y2)/2 -m2*((x1+x2)/2);
	intersect1=(-c1+c2)/(m1-m2);
	intersect2=(c2*m1-m2*c1)/(m1-m2);

	float diffa=x2-x1;
	float diffb=y2-y1;
	float diffc=(x1+x2)/2-intersect1;
	float diffd=(y1+y2)/2-intersect2;
	float vector_res=(diffa*diffd)-(diffb*diffc);
	if(vector_res<0)
	{
		m1=(y2-y1)/(x2-x1);
		deno=sqrt(1+m1*m1);
		c=y1-m1*x1;
		d1=d;
		c1=c+d1*deno;
		if(m1==0)
			m1=0.1;
		m2=-1/(m1);
		c2=(y1+y2)/2 -m2*((x1+x2)/2);
		intersect1=(-c1+c2)/(m1-m2);
		intersect2=(c2*m1-m2*c1)/(m1-m2);
	}


	//if((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0;
	//cout<<intersect1<<endl;

	drawPoint(intersect1,intersect2);

	glColor3f(0.5f, 0.0f, 0.8f);
	drawLine2(x1,x2,y1,y2,0);




	float project_holes_x[n];
	float project_holes_y[n];
	float px=(x2-x1)/n;
	float py=(y2-y1)/n;
	int i;
	for(i=0;i<n;i++){
		project_holes_x[i]=0;
		project_holes_y[i]=0;
	}
	for(i=0;i<n;i++){
		project_holes_x[i]+=x1+i*px;
		project_holes_y[i]+=y1+i*py;
	}
	drawholes(project_holes_x,project_holes_y,n);
	float rr=1,gg=0,bb=0;
	for(i=0;i<n;i++){
		if(colorflag==0)
		{
			if(i<=n/2)
			{
				rr=rr-0.03;
				gg=gg+0.1;
				//int num=rand()%5;
				//float nn=num/10.0;
				rc[i]=rr;
				gc[i]=gg;
				bc[i]=0.0;
			}
			else{
				gg=gg-0.1;
				bb=bb+0.1;
				//int num=rand()%5;
				//float nn=num/10.0;
				rc[i]=0.0;
				gc[i]=gg;
				bc[i]=bb;
			}
		}
		drawLine1(project_holes_x[i],intersect1,project_holes_y[i],intersect2,1,rc[i],gc[i],bc[i]);
	}
	colorflag=1;

	drawRays(project_holes_x,project_holes_y,intersect1,intersect2,n);
}

void drawBlock(float x1,float x2,float y1,float y2)
{
	//glColor3f(0.1f,0.0f,0.9f);
	drawLine2(x1,x2,y1,y2,1);
}

void drawMirror(float x1,float x2,float y1,float y2)
{
	//glColor3f(0.5f,0.4f,0.9f);
	drawLine2(x1,x2,y1,y2,2);
}

void random1(){

	int num,i;
	float num1;
	for(i=0;i<pn;i++)
	{
		int nn=rand()%2;
		if(nn==0){
			num=rand()%100;
			num1=num/70.0;
			int x=rand()%100;
			if(x<=50){
				project_x1[i] += num1;
				project_x2[i] += num1;
			}
			else {
				project_x1[i] -= num1;
				project_x2[i] -= num1;
			}
		}
		else{
			num=rand()%100;
			num1=num/70.0;
			int x=rand()%100;
			if(x<=50){
				project_y1[i] += num1;
				project_y2[i] += num1;
			}
			else {
				project_y1[i] -= num1;
				project_y2[i] -= num1;
			}
		}
	}
	for(i=0;i<block_n;i++)
	{
		num=rand()%100;
		num1=num/150.0;
		int x=rand()%100;
		if(x<=50){
			block_x1[i] += num1;
			block_x2[i] += num1;
		}
		else{
			block_x1[i] -= num1;
			block_x2[i] -= num1;
		}
	}
	for(i=0;i<mirror_n;i++)
	{
		num=rand()%100;
		num1=num/100.0;
		int x=rand()%100;
		if(x<=50){
			mirror_x1[i] += num1;
			mirror_x2[i] += num1;
		}
		else{
			mirror_x1[i] -= num1;
			mirror_x2[i] -= num1;
		}
	}

}

void rotate1(float cdtheta){
	if(keycount<=(pn-1))
	{
		float t1=project_x1[keycount] + (project_x2[keycount]-project_x1[keycount])*cos(cdtheta)-(project_y2[keycount]-project_y1[keycount])*sin(cdtheta);
		float t2=project_y1[keycount] + (project_x2[keycount]-project_x1[keycount])*sin(cdtheta)+(project_y2[keycount]-project_y1[keycount])*cos(cdtheta);
		project_x2[keycount]= t1;
		project_y2[keycount]= t2;
	}
	else if(keycount>=pn && keycount<pn+block_n)
	{
		float t1=block_x1[keycount-pn] + (block_x2[keycount-pn]-block_x1[keycount-pn])*cos(cdtheta)-(block_y2[keycount-pn]-block_y1[keycount-pn])*sin(cdtheta);
		float t2=block_y1[keycount-pn] + (block_x2[keycount-pn]-block_x1[keycount-pn])*sin(cdtheta)+(block_y2[keycount-pn]-block_y1[keycount-pn])*cos(cdtheta);
		block_x2[keycount-pn]= t1;
		block_y2[keycount-pn]= t2;
	}
	else{		
		float t1=mirror_x1[keycount-pn-block_n] + (mirror_x2[keycount-pn-block_n]-mirror_x1[keycount-pn-block_n])*cos(cdtheta)-(mirror_y2[keycount-pn-block_n]-mirror_y1[keycount-pn-block_n])*sin(cdtheta);
		float t2=mirror_y1[keycount-pn-block_n] + (mirror_x2[keycount-pn-block_n]-mirror_x1[keycount-pn-block_n])*sin(cdtheta)+(mirror_y2[keycount-pn-block_n]-mirror_y1[keycount-pn-block_n])*cos(cdtheta);
		mirror_x2[keycount-pn-block_n]= t1;
		mirror_y2[keycount-pn-block_n]= t2;
	}

}

void saveconfig(){
	FILE *fp;
	int i;
	fp= fopen("backup","w");
	fprintf(fp,"%.2f %.2f\n",w,h);
	fprintf(fp,"%d\n",pn);
	for(i=0;i<pn;i++){
		fprintf(fp,"%.2f %.2f %.2f %.2f\n",project_x1[i],project_y1[i],project_x2[i],project_y2[i]);
		fprintf(fp,"%.2f\n",project_d[i]);
		fprintf(fp,"%d\n",project_n[i]);
	}	
	fprintf(fp,"%d\n",block_n);	
	for(i=0;i<block_n;i++){
		fprintf(fp,"%.2f %.2f %.2f %.2f\n",block_x1[i],block_y1[i],block_x2[i],block_y2[i]);
	}	
	fprintf(fp,"%d\n",mirror_n);	
	for(i=0;i<mirror_n;i++)
		fprintf(fp,"%.2f %.2f %.2f %.2f\n",mirror_x1[i],mirror_y1[i],mirror_x2[i],mirror_y2[i]);	
	fclose(fp);
}


void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			saveconfig();
			exit(0);
			break;
		case 'q': //Escape key
			saveconfig();
			exit(0);
			break;
		case 'c':
			keycount+=1;
			if(keycount==total)
				keycount=0;
			break;
		case 'f':
			rotangle+=0.05;
			trgap+=0.1;
			break;
		case 's':
			rotangle-=0.05;
			trgap-=0.1;
			break;
		case 'z':
			uniflag=uniflag*(-1);
			break;
		case 'g':
			gaze=gaze*(-1);
			break;
		case 'l':
			rotate1(rotangle);
			break;
		case 'r':
			rotate1(-rotangle);
			break;
		case 't':
			animation_mode=animation_mode*(-1);
			break;
		case'b':
			total+=1;
			block_n+=1;
			keycount=pn+block_n-1;
			block_x1[block_n-1]=-40.0f;
			block_x2[block_n-1]=-30.0f;
			block_y1[block_n-1]=50.0f;
			block_y2[block_n-1]=35.0f;
			block_slope[block_n-1]=(block_y2[block_n-1]-block_y1[block_n-1])/(block_x2[block_n-1]-block_x1[block_n-1]);
			break;
		case'm':
			total+=1;
			mirror_n+=1;
			keycount=total-1;
			mirror_x1[mirror_n-1]=-40.0f;
			mirror_x2[mirror_n-1]=-30.0f;
			mirror_y1[mirror_n-1]=50.0f;
			mirror_y2[mirror_n-1]=35.0f;
			mirror_slope[mirror_n-1]=(mirror_y2[mirror_n-1]-mirror_y1[mirror_n-1])/(mirror_x2[mirror_n-1]-mirror_x1[mirror_n-1]);
			break;
		case 'p':
			total+=1;
			pn+=1;
			keycount=pn-1;
			project_x1[pn-1]=-30.0f;
			project_x2[pn-1]=-40.0f;
			project_y1[pn-1]=35.0f;
			project_y2[pn-1]=50.0f;
			project_d[pn-1]=4.0;
			project_n[pn-1]=15;
			drawProjector(project_x1[pn-1],project_x2[pn-1],project_y1[pn-1],project_y2[pn-1],project_d[pn-1],project_n[pn-1]);


	}
}



void handleMouseclick(int button, int state, int x, int y){
	if(state == GLUT_DOWN){
		if(button == GLUT_LEFT_BUTTON){
			for(int j=0;j<pn;j++){
				float d1=sqrt((posX-project_x1[j])*(posX-project_x1[j])+(posY-project_y1[j])*(posY-project_y1[j]));
				float d2=sqrt((posX-project_x2[j])*(posX-project_x2[j])+(posY-project_y2[j])*(posY-project_y2[j]));
				float d3=sqrt((project_x1[j]-project_x2[j])*(project_x1[j]-project_x2[j])+(project_y1[j]-project_y2[j])*(project_y1[j]-project_y2[j]));
				//cout<<d1<<" "<<d2<<" "<<d3<<endl;
				if(((d1+d2)<=d3+2)&&((d1+d2)>=d3-2))
				{
				//	cout<<"clicked"<<endl;
					keycount=j;
				}

			}
			for(int j=0;j<block_n;j++){
				float d1=sqrt((posX-block_x1[j])*(posX-block_x1[j])+(posY-block_y1[j])*(posY-block_y1[j]));
				float d2=sqrt((posX-block_x2[j])*(posX-block_x2[j])+(posY-block_y2[j])*(posY-block_y2[j]));
				float d3=sqrt((block_x1[j]-block_x2[j])*(block_x1[j]-block_x2[j])+(block_y1[j]-block_y2[j])*(block_y1[j]-block_y2[j]));
				if((int)(d1+d2)==(int)d3)
				{
				//	cout<<"clicked"<<endl;
					keycount=pn+j;
				}
			}
			for(int j=0;j<mirror_n;j++){
				float d1=sqrt((posX-mirror_x1[j])*(posX-mirror_x1[j])+(posY-mirror_y1[j])*(posY-mirror_y1[j]));
				float d2=sqrt((posX-mirror_x2[j])*(posX-mirror_x2[j])+(posY-mirror_y2[j])*(posY-mirror_y2[j]));
				float d3=sqrt((mirror_x1[j]-mirror_x2[j])*(mirror_x1[j]-mirror_x2[j])+(mirror_y1[j]-mirror_y2[j])*(mirror_y1[j]-mirror_y2[j]));
				if((int)(d1+d2)==(int)d3)
				{
				//	cout<<"clicked"<<endl;
					keycount=pn+block_n+j;
				}
			}


		}
	}
}


void mypassive(int x, int y)
{
	GLint viewport[4];                  // Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves The Viewport Values (X, Y, Width, Height)
	GLdouble modelview[16];                 // Where The 16 Doubles Of The Modelview Matrix Are To Be Stored
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);       // Retrieve The Modelview Matrix
	GLdouble projection[16];                // Where The 16 Doubles Of The Projection Matrix Are To Be Stored
	glGetDoublev(GL_PROJECTION_MATRIX, projection);     // Retrieve The Projection Matrix
 
	GLfloat winX, winY, winZ;               // Holds Our X, Y and Z Coordinates
	 
	winX = x;                  // Holds The Mouse X Coordinate
	winY = y;                  // Holds The Mouse Y Coordinate
	winY = (float)viewport[3] - winY;           // Subtract The Current Mouse Y Coordinate From The Screen Height.
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	cout<<posX<<"   "<<posY<<" "<<posZ<<endl;
}


void handleKeypress2(int key, int x, int y) {
	if(keycount<=(pn-1))
	{

		if (key == GLUT_KEY_LEFT){
			project_x1[keycount] -= trgap;
			project_x2[keycount] -= trgap;
		}
		if (key == GLUT_KEY_RIGHT){
			project_x1[keycount] += trgap;
			project_x2[keycount] += trgap;
		}
		if (key == GLUT_KEY_UP){
			project_y1[keycount] += trgap;
			project_y2[keycount] += trgap;
		}
		if (key == GLUT_KEY_DOWN){
			project_y1[keycount] -= trgap;
			project_y2[keycount] -= trgap;
		}
	}
	else if(keycount>=pn && keycount<pn+block_n)
	{
		if (key == GLUT_KEY_LEFT){
			block_x1[keycount-pn] -= trgap;
			block_x2[keycount-pn] -= trgap;
		}
		if (key == GLUT_KEY_RIGHT){
			block_x1[keycount-pn] += trgap;
			block_x2[keycount-pn] += trgap;
		}
		if (key == GLUT_KEY_UP){
			block_y1[keycount-pn] += trgap;
			block_y2[keycount-pn] += trgap;
		}
		if (key == GLUT_KEY_DOWN){
			block_y1[keycount-pn] -= trgap;
			block_y2[keycount-pn] -= trgap;
		}
	}
	else
	{
		if (key == GLUT_KEY_LEFT){
			mirror_x1[keycount-pn-block_n] -= trgap;
			mirror_x2[keycount-pn-block_n] -= trgap;
		}
		if (key == GLUT_KEY_RIGHT){
			mirror_x1[keycount-pn-block_n] += trgap;
			mirror_x2[keycount-pn-block_n] += trgap;
		}
		if (key == GLUT_KEY_UP){
			mirror_y1[keycount-pn-block_n] += trgap;
			mirror_y2[keycount-pn-block_n] += trgap;
		}
		if (key == GLUT_KEY_DOWN){
			mirror_y1[keycount-pn-block_n] -= trgap;
			mirror_y2[keycount-pn-block_n] -= trgap;
		}

	}



}




//Draws the 3D scene
void drawScene() {
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	glTranslatef(0.0f, 0.0f,zoom);
	for(i=0;i<pn;i++)
		drawProjector(project_x1[i],project_x2[i],project_y1[i],project_y2[i],project_d[i],project_n[i]);
	for(i=0;i<block_n;i++)
		drawBlock(block_x1[i],block_x2[i],block_y1[i],block_y2[i]);
	for(i=0;i<mirror_n;i++)
		drawMirror(mirror_x1[i],mirror_x2[i],mirror_y1[i],mirror_y2[i]);
	if(keycount<=(pn-1))
		highlight(project_x1[keycount],project_x2[keycount],project_y1[keycount],project_y2[keycount]);
	else if(keycount>=pn && keycount<pn+block_n)
		highlight(block_x1[keycount-pn],block_x2[keycount-pn],block_y1[keycount-pn],block_y2[keycount-pn]);
	else		
		highlight(mirror_x1[keycount-pn-block_n],mirror_x2[keycount-pn-block_n],mirror_y1[keycount-pn-block_n],mirror_y2[keycount-pn-block_n]);

	glutSwapBuffers();
}



void update(int value) {
	int i;
	if(gaze==1){
		float slp,mid_x,mid_y;
		for(int i=0;i<pn;i++){
			mid_x=(project_x1[i]+project_x2[i])/2;
			mid_y=(project_y1[i]+project_y2[i])/2;
			slp= atan2((posY-mid_y),(posX-mid_x))-PI/2;
			float l=sqrt(((project_x2[i]-project_x1[i])*(project_x2[i]-project_x1[i]))+((project_y2[i]-project_y1[i])*(project_y2[i]-project_y1[i])));
			project_x2[i]=mid_x+cos(slp)*l/2;
			project_y2[i]=mid_y+sin(slp)*l/2;
			project_x1[i]=mid_x-cos(slp)*l/2;
			project_y1[i]=mid_y-sin(slp)*l/2;
		}
	}
	if(uniflag==1)
		random1();
	stripflag=stripflag*(-1);

	glutPostRedisplay(); //Tell GLUT that the display has changed

	//Tell GLUT to call update again in 25 milliseconds
	glutTimerFunc(250, update, 0);
}

int main(int argc, char** argv) {
	FILE *fil;
	fil= fopen(argv[1],"r");
	fscanf(fil,"%f %f%*[^\n]",&w,&h);
	fscanf(fil,"%d%*[^\n]",&pn);
	total+=pn;
	//printf("%d\n",pn);
	int i,j;
	for(i=0;i<pn;i++){
		fscanf(fil,"%f %f %f %f%*[^\n]",&project_x1[i],&project_y1[i],&project_x2[i],&project_y2[i]);
		//printf("%f %f %f %f",project_x1[i],project_x2[i],project_y1[i],project_y2[i]);
		fscanf(fil,"%f%*[^\n]",&project_d[i]);
		//printf("%f\n",project_d[i]);
		fscanf(fil,"%d%*[^\n]",&project_n[i]);
		//printf("%d\n",project_n[i]);
	}	
	fscanf(fil,"%d%*[^\n]",&block_n);	
	total+=block_n;
	for(i=0;i<block_n;i++){
		fscanf(fil,"%f%f%f%f%*[^\n]",&block_x1[i],&block_y1[i],&block_x2[i],&block_y2[i]);
		//printf("%f%f%f%f\n",block_x1[i],block_x2[i],block_y1[i],block_y2[i]);
	}	
	fscanf(fil,"%d%*[^\n]",&mirror_n);	
	total+=mirror_n;
	for(i=0;i<mirror_n;i++){
		fscanf(fil,"%f%f%f%f%*[^\n]",&mirror_x1[i],&mirror_y1[i],&mirror_x2[i],&mirror_y2[i]);
		//printf("%f%f%f%f[^\n]",mirror_x1[i],mirror_x2[i],mirror_y1[i],mirror_y2[i]);	

	}	
	fclose(fil);

	for(j=0;j<pn;j++){
		if(project_x2[j]==project_x1[j])
			project_x2[j]+=0.1;
		if(project_y2[j]==project_y1[j])
			project_y2[j]+=0.1;
	}

	for(j=0;j<block_n;j++){
		if(block_x2[j]==block_x1[j])
			block_x2[j]+=0.1;
		if(block_y2[j]==block_y1[j])
			block_y2[j]+=0.1;
		block_slope[j]=(block_y2[j]-block_y1[j])/(block_x2[j]-block_x1[j]);
	}

	for(j=0;j<mirror_n;j++){
		if(mirror_x2[j]==mirror_x1[j])
			mirror_x2[j]+=0.1;
		if(block_y2[j]==block_y1[j])
			block_y2[j]+=0.1;
		mirror_slope[j]=(mirror_y2[j]-mirror_y1[j])/(mirror_x2[j]-mirror_x1[j]);
	}


	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w,h);

	//Create the window
	glutCreateWindow("Mirror World");
	initRendering();

	//Set handler functions
	glutDisplayFunc(drawScene);
	//glutIdleFunc (drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleKeypress2);
	glutReshapeFunc(handleResize);
	glutMouseFunc(handleMouseclick);
	glutPassiveMotionFunc(mypassive);
	glutTimerFunc(25, update, 0); //Add a timer

	glutMainLoop();
	return 0;
}









