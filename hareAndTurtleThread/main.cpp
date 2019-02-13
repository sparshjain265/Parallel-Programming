#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>

using namespace std;

//step time in milli seconds
#define step_time 50

//report delay in milli seconds
#define report_delay 250

//winning position
#define win_pos 500

//well ahead
#define well_ahead 50

int hare_pos = 0;
int turtle_pos = 0;	

//this is to prevent two threads accessing the position variable at the same time
shared_mutex hare_pos_m;
shared_mutex turtle_pos_m;

//used by all threads to conclude whether the process has finished
shared_mutex finished_m;
bool finished = false;

void hare()
{
    int step_size;
	int sleep_time;
    while(true)
    {
		//check for finish
		finished_m.lock_shared();
		if(finished)
		{
			finished_m.unlock_shared();
			break;
		}
		else
			finished_m.unlock_shared();

		//before moving ahead, check if hare is well ahead of turtle
		//if yes, sleep for a random time
		hare_pos_m.lock_shared();
		turtle_pos_m.lock_shared();
		if(hare_pos - turtle_pos > well_ahead)
		{
			hare_pos_m.unlock_shared();
			turtle_pos_m.unlock_shared();
			sleep_time = well_ahead + 10 * (rand() % 10);
			this_thread::sleep_for(chrono::milliseconds(step_time*sleep_time));
		}
		else
		{
			hare_pos_m.unlock_shared();
			turtle_pos_m.unlock_shared();
		}
		
		//move ahead
		step_size = 2 + rand() % 5;	//between 2 and 6 inclusive
		hare_pos_m.lock();
		hare_pos += step_size;
		if(hare_pos > win_pos)
		{
			hare_pos_m.unlock();
			finished_m.lock();
			finished = true;
			finished_m.unlock();
		}
		hare_pos_m.unlock();
		
		//delay before taking next step
		this_thread::sleep_for(chrono::milliseconds(step_time));
    }
}

void turtle()
{
    while(true)
    {
		//check for finish
		finished_m.lock_shared();
		if(finished)
		{
			finished_m.unlock_shared();
			break;
		}
		else
			finished_m.unlock_shared();


		//move ahead
		turtle_pos_m.lock();
		turtle_pos += 1;
		if(turtle_pos > win_pos)
		{
			turtle_pos_m.unlock();
			finished_m.lock();
			finished = true;
			finished_m.unlock();
		}
		turtle_pos_m.unlock();
		
		//delay before taking next step
		this_thread::sleep_for(chrono::milliseconds(step_time));
    }
}

void reporter()
{
    while(true)
    {
		//report position
		hare_pos_m.lock_shared();
		turtle_pos_m.lock_shared();
		printf("Hare: %d\t Turtle: %d\n", hare_pos, turtle_pos);

		//check for win and update finish accordingly
		if (turtle_pos > win_pos) 
		{
			hare_pos_m.unlock_shared();
			turtle_pos_m.unlock_shared();

			finished_m.lock();
			finished = true;
			finished_m.unlock();

			printf("Turtle Won! Enter q to exit!\n");
			break;
		}
		else if (hare_pos > win_pos)
		{
			hare_pos_m.unlock_shared();
			turtle_pos_m.unlock_shared();

			finished_m.lock();
			finished = true;
			finished_m.unlock();

			printf("Hare Won! Enter q to exit!\n");
			break;
		}
		else
		{
			hare_pos_m.unlock_shared();
			turtle_pos_m.unlock_shared();
		}
		
		//check for finish
		finished_m.lock_shared();
		if(finished)
		{
			finished_m.unlock_shared();
			break;
		}
		else
			finished_m.unlock_shared();

		//wait before reporting again
		this_thread::sleep_for(chrono::milliseconds(report_delay));
    }    
}

int main()
{
	//seed rand()
    srand(time(NULL));

	//start the race
    thread hare_t(hare);
    thread turtle_t(turtle);
    thread reporter_t(reporter);

    char option;
    int h_pos,t_pos;

    printf("Starting..\n Enter q to exit\n Enter p hare_pos turtle_pos to reposition\n");

    while(true)
    {
		cin>>option;
        if(option=='q')
        {
			finished_m.lock();
    		finished = true;
    		finished_m.unlock();
            break;
        }
        else if(option == 'p')
		{
			cin>>h_pos>>t_pos;
            hare_pos_m.lock();
            hare_pos = h_pos;
            hare_pos_m.unlock();
            turtle_pos_m.lock();
            turtle_pos = t_pos;
            turtle_pos_m.unlock();
        }
    }

    hare_t.join();
    turtle_t.join();
    reporter_t.join();
    return 0;
}
