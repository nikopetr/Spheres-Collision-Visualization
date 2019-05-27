#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

char SpheresCrash(double r1,double r2,double x1,double x2,double y1,double y2)//Σύναρτηση που ελέγχει αν γίνεται κρούση δύο σφαίρων
{   double distance;
    distance=sqrt(pow((x2-x1),2)+pow((y2-y1),2));//Υπολογισμός της αποόστασης μεταξύ τους
    if (distance<=(r1+r2))//Αν όντως γίνεται η κρούση επιστρέφει true
        return 1;

    return 0;//Αν όχι επιστρέφει 0
}

void ApplyImpact (double *speed1,double *speed2,double m1,double m2)//Συνάρτηση που άλλαζει τις ταχύτητες των σφαιρών όταν υπάρξει κρούση
{
    double TempSp1=*speed1;//Αποθηκεύονται σε μεταβλητές προσωρινά οι αρχικές ταχύτητες
    double TempSp2=*speed2;
    *speed1=((2*m2/(m1+m2))*TempSp2)+(((m1-m2)/(m1+m2))*TempSp1);//Αλλαγή ταχύτητας πρώτης σφαίρας
    *speed2=((2*m1/(m1+m2))*TempSp1)+(((m2-m1)/(m1+m2))*TempSp2);//Αλλαγή ταχύτητας δεύτερης σφαίρας

    return ;
}

int main()
{   clock_t start,finish, previous;
    FILE *fp;
    int n,i,j;
    char CheckAlloc,CheckValue=1;
    double step,*x,*y,*m,*r,*sx,*sy;
    int *c;
    /*Το x και y είναι οι θέσεις της σφαίρας στους άξονες.
      Το c είναι το χρώμα της σφαίρας.
      r η ακτίνα σφαίρας.
      m η μάζα της σφαίρας.
      sx και sy η ταχύτητα της σε pixels/sec.*/

    if ((fp=fopen("input.txt","r"))!=NULL)//Άνοιγμα αρχείου input.txt για ανάγνωση
  {
       fscanf(fp,"%d",&n);
       if (n<=0) //Ελέγχω αν το ο αριθμός τον σφαιρών είναι θετικός
       CheckValue=0;

       x=(double *)malloc(n*sizeof(double));//Δέσμευση χώρου δυναμικά για τα στοιχέια της κάθε σφαίρας και ελέγχος αν έγινε σωστά
       if (x==NULL)
        CheckAlloc=0;
       y=(double *)malloc(n*sizeof(double));
       if (y==NULL)
        CheckAlloc=0;
       c=(int *)malloc(n*sizeof(int));
       if (c==NULL)
        CheckAlloc=0;
       m=(double *)malloc(n*sizeof(double));
       if (m==NULL)
        CheckAlloc=0;
       r=(double *)malloc(n*sizeof(double));
       if (r==NULL)
        CheckAlloc=0;
       sx=(double *)malloc(n*sizeof(double));
       if (sx==NULL)
        CheckAlloc=0;
       sy=(double *)malloc(n*sizeof(double));
       if (sy==NULL)
        CheckAlloc=0;

       if (!CheckAlloc)
        printf("Could not allocate memory\n");
       else
        {
          for (i=0;i<n&&CheckValue;i++)
           {
             fscanf(fp,"%lf%lf%d%lf%lf%lf%lf",&x[i],&y[i],&c[i],&m[i],&r[i],&sx[i],&sy[i]);//Ανάγνωση τιμών από αρχείο
             if (x[i]>=0&&x[i]<=600&&y[i]>=0&&y[i]<=400&&c[i]>=0&&c[i]<=15&&m[i]>=0&&m[i]<=10000&&r[i]>=0&&r[i]<=100//Ελεγχος τιμών
                 &&sx[i]>=-100&&sx[i]<=100&&sy[i]>=-100&&sy[i]<=100)
                 CheckValue=1;
             else
                 CheckValue=0;
           }
        }
        fclose(fp);//Κλείσιμο αρχείου
  }
   else
       printf("Error opening file\n");

   if (!CheckValue)
   {
       printf("Wrong Input\n");
       exit(1);
   }

   /* request autodetection */
    int gdriver = DETECT, gmode, errorcode;// Γίνεται ενεργοποιήση των γραφικών
    initgraph(&gdriver, &gmode, "");// Η μεταβλητή gdriver παίρνει τιμή για το ανάλογο driver, gmode για την καλύτερη ανάλυση που υποστηρίζεται
    errorcode = graphresult();
    if (errorcode != grOk)
    {
       /*Σε περίπτωση προβλήματος με τα γραφικά θά εμφανίζεται το αντίστοιχο μήνυμα για το συγκεκριμένο πρόβλημα στον χρήστη*/
       printf("Graphics error: %s\n", grapherrormsg(errorcode));
       system ("pause");
       exit(1);
    }

    start = clock();
    previous = start;

    do
    {
        finish = clock();
        step = (finish-previous)*1.0/CLOCKS_PER_SEC;

        if (step>=0.005)
        {
           previous = finish;

           for (i=0;i<n;i++)
           {
              setfillstyle(SOLID_FILL,BLACK);//μαυρίζεται η προηγούμενη τοποθεσία της σφαίρας για να σβηστεί
              setcolor(BLACK);
              fillellipse(x[i],y[i],r[i],r[i]);

              x[i]+= sx[i]*step;//Μετακίνηση σφαίρας
              y[i]+= sy[i]*step;

              if (x[i]+r[i]>=getmaxx() || x[i]-r[i]<=0)//Συνθήκη όταν η σφαίρα χτυπήσει σε οριζόντιο τοίχο
                  sx[i] *= -1;
              if (y[i]+r[i]>=getmaxy() || y[i]-r[i]<=0)//Συνθήκη όταν η σφαίρα χτυπήσει σε κάθετο τοίχο
                  sy[i] *= -1;

              setfillstyle(SOLID_FILL,c[i]);// Καθορίζεται χρώμα στη σφαίρα και "ζωγραφίζεται"
              setcolor(c[i]);
              fillellipse(x[i],y[i],r[i],r[i]);
           }

           for (i=0;i<n;i++)
             for (j=i+1;j<n;j++)
                if (SpheresCrash(r[i],r[j],x[i],x[j],y[i],y[j]))//Ελεγχός μέσω συνάρτησης αν έχει γίνει κάποια κρούση
                   {
                     ApplyImpact(&sx[i],&sx[j],m[i],m[j]);//Αλλαγή ταχύτητας στον άξονα Χ μέσω συνάρτησης
                     ApplyImpact(&sy[i],&sy[j],m[i],m[j]);//Αλλαγή ταχύτητας στον άξονα y μέσω συνάρτησης

                   }// Τέλος έλεγχου if για κρούση

         }// Τέλος έλεγχου if  για step

      }// Τέλος βρόγχου Do

    while (!kbhit());//Συνεχίζονται οι κανονικά επαναλήψεις μέχρι ο χρήστης να πατήσει κάποιο πλήκτρο.
    closegraph();
    printf("Hope you liked it !!!\n");
   }
