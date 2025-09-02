#include <stdio.h>

int n_dise(int n)
{                                       //From rand()%n they generate a random number between 0 to n-1
    return (rand() % n) + 1;            //by adding 1 to it we can get a random number between  1 to n
}

void playnow(int x,int pos[4][4],int i,int relpos_open[],char *playerorder[])
{
    int die=n_dise(6);          //generating a random value between 1 to 6
    int count;
    int count4;
    if (die == 6 && x < 2)      //if the geneted value = 6 and the repeating dise trow <=2
    {
        count=0;
        count4=0;
        for(int j=0;j<4;j++)
        {
            if(pos[i][j]>1 && pos[i][j]<53)         
            {
                count++;        //counting the no. of pieces who can move 6 spaces
            }
            if(pos[i][j]==1)
            {
                count4++;       //counting the no. of pieces who are in the Base.
            }
        }
        int choice=for_the_base_or_to_the_board(count,count4);      //asking the choice from functin for_the_base_or_to_the_board
        if (choice==1)      //If the choice is to take a piece from the base
        {
            int j;
            while(1)         //start a infint loop and run it until you find a piece in the base.
            {
                j=(rand() % 4);
                if(pos[i][j]==1) //if the piece is in the base(if a piece is in the base possition is=1)
                {
                    pos[i][j]=2;//taking the random piece to the starting point(if the possition is = 2 it is in its start) and braeking the infinit loop.
                    break;  
                }
            } 
            int count=0;
            for(int k=0;k<4;k++)
            {                           //count the no. of players in the base
                if(pos[i][k]==1)
                {
                    count++;
                }
            }       //displaying player is taking a piece from base
            printf("%s Player rolled %d.\n",playerorder[i] ,die);
            printf("%s Player moves peice %s-%d to the starting point.\n",playerorder[i],playerorder[i],j+1);
            printf("%s Player now has %d/4 pieces on the board and %d/4 pieces on the base.\n",playerorder[i],4-count,count);
            printf("%s Player Rolled 6. So he will get an another roll.\n\n",playerorder[i]);
            x=x+1;      //increment the repeating dise trow
            playnow(x,pos,i,relpos_open,playerorder);       //call playnow function again because player rolled 6 and he will get an another trow
        }
        else            //If the choice is to take the value to board
        {
            x=x+1;      //increment the repeating dise trow
            printf("%s Player rolled %d\n",playerorder[i] ,die);
            playnow_3(i,pos,playerorder,relpos_open,die);       //calling function playnow_3 to continue the game
            printf("%s Player Rolled 6. So he will get an another roll.\n\n",playerorder[i]);
            playnow(x,pos,i,relpos_open,playerorder);       //call playnow function again because player rolled 6 and he will get an another trow
        }
    }
     else   //else the geneted value = 6 and the repeating dise trow <=2
    {
        if (die == 6) 
        {
            printf("%s Player rolled %d\n",playerorder[i] ,die);
            die = 0;            //the dise value will be terminated because he rolled 6 three times in a row.
            printf("%s rolled value 6,three Times in a Row.So The Last Trow Will Be Terminated.....\n",playerorder[i]);
        }
        else        //if dise value is not equal to 6
        {
            printf("%s Player rolled %d\n",playerorder[i] ,die);
            playnow_3(i,pos,playerorder,relpos_open,die);       //calling function playnow_3 to continue the game
        } 
    }
}


int main(){

    return 0;
}