uint8_t Buffer = 0;
uint8_t Error = 0;
uint8_t BTN = 0;
uint8_t key[3] = {0};

int main()
{
  DDRD |= (1<<3);//TX
  DDRD &= ~(1<<4);//RX(Test)
  //PORTD &= ~(1<<4);//RX
  PORTD |= (1<<3);//Подтягивание TX к питанию для блока сигнала
  //[7]128   [6]64   [5]32   [4]16   [3]8   [2]4   [1]2   [0]1
  
  Serial.begin(9600);
  while(1)
  {
    Listen();
    CatchKey();
    //Work();
  }
}

/********************************************/
void Byte(uint8_t data){//Запись байта
  PORTD &= ~(1<<3);//TX опускаем
  /************/
  
  COPY_delay_loop_2(413);
  /************/
    for(uint8_t i = 8;i > 0;i--){
    if((data & 1) != 0){
      PORTD |= (1<<3);
    }
    else{
      PORTD &= ~(1<<3);
    }
    COPY_delay_loop_2(413);
    data >>= 1;
  }
  PORTD |= (1<<3);//Стоп бит
  COPY_delay_loop_2(414);
}
/***************************************************/
void Str(uint8_t *str)//Запись строк
{
  uint8_t i = 0;
  while(str[i] !=0)
  {
    Byte(str[i]);
    i++;
  }
}
/*****************************************************/

void Listen ()
{
  bool Uspeh,Start;
  uint16_t count = 0;
  uint8_t data = 0;
  uint8_t BTN = 0;
  while(count < 255)
  {
    if((PIND & 16) != 0)
    {
      count++;
    }
    else
    {
      COPY_delay_loop_2(618);//Задержка 3/2 от 104 мкс |||Задержка 1/2 от 104 мкс old 206
      for(uint8_t i = 0;i < 8; i++)//Чтение 8 битов
      { 
        if((PIND & 16) != 0)
        {
          data |= (1<<i);
        }
        else
        {
          data &= ~(1<<i);
        }
        COPY_delay_loop_2(413);
      }
      
      COPY_delay_loop_2(206);//Прыгнуть на середину стоп-бита
      
      if((PIND & 16) ? Uspeh = true : Uspeh = false) //Видим стоп бит
      {
          if(Uspeh)
          {
              Buffer = data;//Сохраняем байт
          } 
          else
          {
             Error++;//Показываем сколько раз была ошибка
          }
      }
    }
  }
  return 0;  
}

void COPY_delay_loop_2(uint16_t __count)
{
  __asm__ volatile (
    "1: sbiw %0,1" "\n\t"
    "brne 1b"
    : "=w" (__count)
    : "0" (__count)
  );
}
