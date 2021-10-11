volatile uint8_t pcount;

/*uint16_t subtract_cap(uint16_t num,uint16_t sub){
if (num > sub)
    return num - sub;
  else
    return 1;
}*/


//uint8_t centr = subtract_cap(416/ 2, 19); //[189]

//uint16_t bit_delay = (F_CPU / 9600) / 4; //=[416]
//uint16_t count = subtract_cap(bit_delay, 15 / 4); //= [413]*/ 416 - 3

uint8_t Buffer = 0;
int main() {
  
  DDRD |= (1<<3);//TX
  DDRD &= ~(1<<4);//RX(Test)
  PORTD &= ~(1<<4);//RX
  PORTD |= (1<<3);//Подтягивание TX к питанию для блока сигнала


                       
  while (1) {
    Str("Succses transfer");
   
  }
  return 0;
}
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
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
  COPY_delay_loop_2(416);
}

void Str(uint8_t *str)//Запись строк
{
  uint8_t i = 0;
  while(str[i] !=0)
  {
    Byte(str[i]);
    i++;
  }
}

void Listen ()
{
  bool listening;
  uint8_t data = 0;
  
  if(!(PIND & 16)) listening = true; //Видим старт бит
    
  COPY_delay_loop_2(206);//Задержка 1/2 от 104 мкс 
  
  for(uint8_t i = 0;i < 8; i++)//Чтение 8 битов
  {
    if((PIND & 16) != 0){
      data |= 128;
    }
    data >>= 1;
    COPY_delay_loop_2(413);
  }
  
  if((PIND & 16) != 0)listening = false; //Видим стоп бит
  Buffer = data;//Сохраняем бит
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
