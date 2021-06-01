//Mateus Conte Feitosa - 22.118.129-0
//Guilherme Couto Feitosa - 22.118.080-5

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <time.h>

using namespace std;

#define Components 11

//Define o tempo de cada ciclo.
int temp = 1000;

//Define as variáveis e seus respectivos valores.
int Component[Components];
float etoh, naoh, oil;
float reactor[3] = {0, 0, 0};
float decanter = 0;
float glycerin, biodiesel = 0;
float dryerSolution, washSolution = 0;
bool activeWash[3] = {false, false, false};
float firstWash, secondWash, thirdWash = 0;
bool activeDecanter = true;
float etohDryer = 0;
bool activeEtOHDryer = true;
bool activeBiodieselDryer = false;
bool reactorTankFilled = false;
int cycles = 0;
bool activeReactor = false;
bool activatedWash = false;

//Inicializa a condição da Thread em relação aos métodos.
pthread_cond_t condition[Components];

//Define os métodos.
void *threadComponent(void *arg);
void residualOil();
void oilTank();
void reactorTank();
void decanterTank();
void decanterDryer();
void naohEtohTank();
void firstWashTank();
void secondWashTank();
void thirdWashTank();
void biodieselDryer();

//Função principal.
int main(int argc, char *argv[])
{

  pthread_t ph_thread[Components];

  for (int i = 0; i < Components; i++)
  {
    pthread_cond_init(&condition[i], NULL);
    Component[i] = i;
  }

  for (int i = 0; i < Components; i++)
  {
    pthread_create(&ph_thread[i], NULL, threadComponent, &Component[i]);
    sleep(0);
  }

  for (int i = 0; i < Components; i++)
    pthread_join(ph_thread[i], NULL);

  for (int i = 0; i < Components; i++)
    pthread_cond_destroy(&condition[i]);

  return 0;
}

//Componente para definição de cada thread.
void *threadComponent(void *arg)
{
  int threadValue = *(int *)arg;

  while (cycles <= 3600)
  {
    if (threadValue == 0)
    {
      cycles++;
      usleep(temp * 1000);
      cout << "Cycle number " << cycles << endl << endl;
      cout << "Biodiesel: " << biodiesel << endl;
      cout << "Glycerin: " << glycerin << endl;
      cout << "Oil: " << oil << endl;
      cout << "NaOH: " << naoh << endl;
      cout << "EtOH: " << etoh << endl;
      cout << "Reactor Oil: " << reactor[0] << endl;
      cout << "Reactor NaOH: " << reactor[1] << endl;
      cout << "Reactor EtOH: " << reactor[2] << endl;
      cout << "Decanter value: " << decanter << endl;
      cout << "___________________" << endl << endl;
    }
    if (threadValue == 1)
    {
      residualOil();
    }
    if (threadValue == 2)
    {
      oilTank();
    }
    if (threadValue == 3)
    {
      reactorTank();
    }
    if (threadValue == 4)
    {
      decanterTank();
    }
    if (threadValue == 5)
    {
      naohEtohTank();
    }
    if (threadValue == 6)
    {
      firstWashTank();
    }
    if (threadValue == 7)
    {
      secondWashTank();
    }
    if (threadValue == 8)
    {
      thirdWashTank();
    }
    if (threadValue == 9)
    {
      biodieselDryer();
    }
    if (threadValue == 10)
    {
      decanterDryer();
    }
  }

  return 0;
}

//Método do óleo residual que calcula seu valor para cada ciclo.
void residualOil()
{
  int time = (rand() % (temp * 1000 * 10));
  usleep(time);
  cout << "(Oil entrance)\n" << endl;
  float aux = rand() % 1000;
  float oilVolume = 1.0 + (aux / 1000);
  oil += oilVolume;
  usleep((temp * 10000) - time);
  cout << "(Oil received)\n" << endl;
}

//Método do óleo do tanque que calcula a quantidade que o óleo ficará a cada ciclo.
void oilTank()
{
  if (reactor[0] < 1.25 && oil >= 1.25)
  {
    float oilReactor = reactor[0];
    float remainingOil = 1.25 - oilReactor;
    reactor[0] += remainingOil;
    oil -= remainingOil;
  }
}

//Método do tanque do reator que calcula o valor de seu volume para cada ciclo.
void reactorTank()
{
  if (reactor[0] >= 1.25 && reactor[1] >= 1.25 && reactor[2] >= 2.50)
  {
    reactorTankFilled = true;
    if (!activeReactor)
      cout << "The reactor throwed the chemical products to the decanter." << endl << endl;
    activeReactor = true;
  }

  usleep(temp * 1000);
}

//Método do decantador para atualizar e calcular seu valor a cada ciclo.
void decanterTank()
{
  if (decanter < 10.0 && reactorTankFilled)
  {
    float decanterCopy = decanter;
    float remainingDecanter = 10.0 - decanter;

    decanter += 5.0;
    reactor[0] = 0;
    reactor[1] = 0;
    reactor[2] = 0;

    usleep(temp * 5000 * 1.66);

    reactorTankFilled = false;
  }

  if (decanter > 0 && activeDecanter == true)
  {
    glycerin += decanter * 0.02;
    etohDryer += decanter * 0.09;
    washSolution += decanter * 0.89;

    decanter = 0;
    activeWash[0] = true;
    activeEtOHDryer = true;
    activeDecanter = false;

    cout << "The decanter throwed the chemical products." << endl << endl;
  }
}

//Método do secador do decantador que ativa o secador do EtOH e atualiza seu valor.
void decanterDryer()
{
  if (activeEtOHDryer == true)
  {
    cout << "Dryer EtOH for EtOH" << endl << endl;
    etoh += etohDryer - (etohDryer * 0.03);
    etohDryer = 0;
    activeEtOHDryer = false;
    usleep(temp * 5000);
  }
}

//Método do tanque de NaOH e EtOH que inicializa seus valores e os atualiza a cada ciclo.
void naohEtohTank()
{
  usleep(temp * 1000);
  naoh += 0.25;
  etoh += 0.125;

  if (reactor[1] < 1.25 && naoh >= 1.25)
  {
    float naohReactor = reactor[1];
    float remainingNaOH = 1.25 - naohReactor;
    reactor[1] += remainingNaOH;
    naoh -= remainingNaOH;
  }
  if (reactor[2] < 2.50 && etoh >= 2.50)
  {
    float etohReactor = reactor[2];
    float remainingEtOH = 2.50 - etohReactor;
    reactor[2] += remainingEtOH;
    etoh -= remainingEtOH;
  }
}

//Método onde ocorre a primeira lavagem e mostra quando foi ativada.
void firstWashTank()
{
  if (activeWash[0] && washSolution > 0)
  {
    cout << "Wash solution: " << washSolution << endl << endl;
    activeDecanter = false;
    firstWash += washSolution - (washSolution * 0.075);
    washSolution -= washSolution * 0.075;
    activeWash[0] = false;
    usleep(temp * 1000);
    activeWash[1] = true;

    if (!activatedWash)
    {
      cout << "The wash solution has been activated." << endl << endl;
      activatedWash = true;
    }
  }
}

//Método onde ocorre a segunda lavagem e mostra quando foi ativada.
void secondWashTank()
{
  if (activeWash[1])
  {
    firstWash = 0;
    secondWash += washSolution - (washSolution * 0.075);
    washSolution -= washSolution * 0.075;
    activeWash[1] = false;
    usleep(temp * 1000);
    activeWash[2] = true;
    cout << "\nSecond wash: " << secondWash << endl;
  }
}

//Método onde ocorre a terceira lavagem e mostra quando foi ativada.
void thirdWashTank()
{
  if (activeWash[2])
  {
    secondWash = 0;
    thirdWash += washSolution - (washSolution * 0.075);
    washSolution -= washSolution * 0.075;
    activeWash[2] = false;
    cout << "Third wash: " << thirdWash << endl << endl;
    usleep(temp * 1000);
    activeBiodieselDryer = true;
    dryerSolution = thirdWash;
    thirdWash = 0;
  }
}

//Método do secador de biodisel que calcula seu valor a cada ciclo.
void biodieselDryer()
{
  if (activeBiodieselDryer)
  {
    biodiesel += dryerSolution - (dryerSolution * 0.03);
    cout << "\n(Sending the dryer solution)\n" << endl;
    usleep(temp * dryerSolution * 5000);
    cout << "(Biodiesel entrance)" << endl;
    activeBiodieselDryer = false;
    activeDecanter = true;
  }
}
