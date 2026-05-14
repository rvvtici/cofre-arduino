#include <LiquidCrystal.h>
#include <Servo.h>

// acho q no readme vou ir comentando cada funcao, setup, loop.
//ver os leds vermelhos e o buzzer

//estados
#define TRANCADO 0
#define SENHA 1
#define ABERTO 2
#define ALERTA 3
#define BLOQUEADO 4

int estadoAtual = TRANCADO;
int estadoAnterior = -1;


//pinos
LiquidCrystal lcd_1(12,11,7,6,5,4);

int buzzer = A1;
int led_vermelho = A2;
int led_verde = A3;
int fotossensor = A0;

Servo servomotor;
int servomotor_pin = 13;

int primeiro_botao = 10;
int segundo_botao = 9;
int terceiro_botao = 8;
int quarto_botao = 3;
int reset_botao = 2;
int enter_botao = A4;

//variáveis globais
const int servomotor_aberto = 90;
const int servomotor_fechado = 0;

volatile int primeiro_digito = 0;
volatile int segundo_digito  = 0;
volatile int terceiro_digito = 0;
volatile int quarto_digito   = 0;

unsigned long timer = 0;
unsigned long timer_senha = 0;

const unsigned long tempo_senha = 10000;
const unsigned long tempo_aberto = 10000;
const unsigned long tempo_bloqueio = 10000;
const unsigned long tempo_alerta = 2000;

const int senha[4] = {1, 3, 2, 6};
int tentativas = 0;
int contagem_bloqueado = 0;
float luz = 0.0;



void setup() {
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  
  servomotor.attach(servomotor_pin);
  servomotor.write(servomotor_fechado);

  pinMode(primeiro_botao, INPUT_PULLUP);
  pinMode(segundo_botao, INPUT_PULLUP);
  pinMode(terceiro_botao, INPUT_PULLUP);
  pinMode(quarto_botao, INPUT_PULLUP);
  pinMode(enter_botao, INPUT_PULLUP);
  pinMode(reset_botao, INPUT_PULLUP);
  
  
  pinMode(fotossensor, INPUT);

  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(buzzer, OUTPUT);
}



void loop() {
  // clear toda vez que o estado muda
  if (estadoAtual != estadoAnterior) {
    lcd_1.clear();
    estadoAnterior = estadoAtual;
  }
  
  Serial.println(luz);
  
  //cada estado tem atribuição à sua respectiva função
  switch (estadoAtual) {
    case TRANCADO: FuncaoTrancado(); break;
    case SENHA: FuncaoSenha(); break;
    case ABERTO: FuncaoAberto(); break;
    case ALERTA: FuncaoAlerta(); break;
    case BLOQUEADO: FuncaoBloqueado(); break;
  }
}



void FuncaoTrancado() {
  //setup do estado TRANCADO
  lcd_1.setCursor(4, 0);
  lcd_1.print("TRANCADO");
  digitalWrite(led_vermelho, HIGH);
  digitalWrite(led_verde, LOW);
  noTone(buzzer);
  
  //se qualquer botão for pressionado, muda p/ estado SENHA
  if (digitalRead(primeiro_botao) == LOW 
      || digitalRead(segundo_botao)  == LOW
      || digitalRead(terceiro_botao) == LOW
      || digitalRead(quarto_botao)   == LOW
      || digitalRead(reset_botao)    == LOW
      || digitalRead(enter_botao)    == LOW) 
  { // antes de mudar para o estado SENHA, já incrementa o dígito
      if (digitalRead(primeiro_botao) == LOW) incrementarPrimeiro();
      if (digitalRead(segundo_botao)  == LOW) incrementarSegundo();
      if (digitalRead(terceiro_botao) == LOW) incrementarTerceiro();
      if (digitalRead(quarto_botao)   == LOW) incrementarQuarto();
      
      delay(150); // delay para o botão não ser pressionado várias vezes
      timer_senha  = millis(); // a variável timer_senha salva o momento em que o botão foi pressionado
      estadoAtual  = SENHA; // muda de estado
  }
}




void FuncaoSenha() {
  //setup do estado SENHA
  lcd_1.setCursor(5, 0);
  lcd_1.print("SENHA:");
  digitalWrite(led_vermelho, HIGH);
  digitalWrite(led_verde, LOW);
  
  lcd_1.setCursor(6,1);
  lcd_1.print(primeiro_digito);
  lcd_1.print(segundo_digito);
  lcd_1.print(terceiro_digito);
  lcd_1.print(quarto_digito);

  delay(250); // delay para o botão não ser pressionado várias vezes
  
  // cada botão e sua respectiva função
  if(digitalRead(primeiro_botao) == LOW) {
    incrementarPrimeiro();
  } if (digitalRead(segundo_botao) == LOW){
    incrementarSegundo();
  } if (digitalRead(terceiro_botao) == LOW) {
    incrementarTerceiro();
  } if (digitalRead(quarto_botao) == LOW) {
    incrementarQuarto();
  } if (digitalRead(enter_botao) == LOW) {
  	enter();
  } if (digitalRead(reset_botao) == LOW) {
  	reset();
  }
  
  // timeout de inatividade
  bool input = digitalRead(primeiro_botao) == LOW
               || digitalRead(segundo_botao) == LOW
               || digitalRead(terceiro_botao) == LOW
               || digitalRead(quarto_botao) == LOW
               || digitalRead(enter_botao) == LOW
               || digitalRead(reset_botao) == LOW;

  if (input) { // qnd qlqr botão é pressionado, salva o momento na variável timer_senha
    timer_senha = millis(); //variável timer_senha obtém o segundo atual
  }

  //obtem segundo atual e compara com timer_senha. caso tenha passado de 10s, volta ao estado TRANCADO  
  if (millis() - timer_senha >= tempo_senha) {
  	reset();
    estadoAtual = TRANCADO;
    return;
  }
}




void FuncaoAlerta() {
  //setup função ALERTA
  lcd_1.setCursor(0, 0);
  lcd_1.print("SENHA INCORRETA:");
  lcd_1.setCursor(6,1);
  lcd_1.print(tentativas);
  lcd_1.print("/3");
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, HIGH);
  
  // pisca o LED vermelho a cada 300ms
  if ((millis() / 300) % 2 == 0) {
    digitalWrite(led_vermelho, HIGH);
  } else {
    digitalWrite(led_vermelho, LOW);
  }

  if (tentativas >= 3) { // após três tentativas falhas, muda para o estado BLOQUEADO
    contagemEstado(tempo_alerta, BLOQUEADO); //countdown (fica no estado ALERTA por 2s)
  } else { // menos de três tentativas: volta pro estado SENHA
    contagemEstado(tempo_alerta, SENHA);
  }
}



void FuncaoBloqueado() {
  //setup estado BLOQUEADO
  lcd_1.setCursor(3, 0);
  lcd_1.print("BLOQUEADO");
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  
  // LED vermelho piscando a cada 300ms
  if ((millis() / 300) % 2 == 0) {
    digitalWrite(led_vermelho, HIGH);
  } else {
    digitalWrite(led_vermelho, LOW);
  }

  // buzzer bipando a cada 500ms
  if ((millis() / 500) % 2 == 0) {
    tone(buzzer, 800);
  } else {
    noTone(buzzer);
  }

  //display mostra countdown de 10s até 00s
  int restante = 10 - ((millis() - timer) / 1000);
  lcd_1.setCursor(6, 1);
  if (restante < 10) {
    lcd_1.print("0");
  }
  lcd_1.print(restante);
  lcd_1.print("s ");

  // depois de 10s, volta para o estado TRANCADO
  if (millis() - timer >= tempo_bloqueio) {
    tentativas = 0;
    digitalWrite(led_vermelho, LOW);
    noTone(buzzer);
    estadoAtual = TRANCADO;
    timer = millis();
  }
}



void FuncaoAberto() {
  //setup estado ABERTO
  lcd_1.setCursor(5, 0);
  lcd_1.print("ABERTO");
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_verde, HIGH);
  noTone(buzzer);
  luz = analogRead(fotossensor);
  //Serial.println(luz);
  
  //durante os primeiros 10s não verifica a luz
  if (millis() - timer < tempo_aberto) {
    return;
  }
  
  // depois de 10s verifica a luz. se tiver luz chegando dentro do cofre, continua aberto
  if (luz < 800) {
    return;
  }

  // sem luz -> apos 5s fecha cofre
  delay(5000);
  servomotor.write(servomotor_fechado);
  reset();
  estadoAtual = TRANCADO;
  timer = millis();
}



//obtem segundo atual e compara com timer_senha. caso tenha passado de 10s, volta ao estado TRANCADO
void contagemEstado(unsigned long tempo, int proximoEstado) {
  if (millis() - timer >= tempo) {
    timer = millis();
    estadoAtual = proximoEstado;
  }
}



// função caso o botão enter seja pressionado. verifica os dígitos e compara com a senha
void enter() {
  if ((primeiro_digito == senha[0])
      && (segundo_digito  == senha[1])
      && (terceiro_digito == senha[2])
      && (quarto_digito   == senha[3])) 
  { // senha correta
    tone(buzzer, 100, 150);
    delay(150);
    reset();
    servomotor.write(servomotor_aberto);
    timer = millis();
    estadoAtual = ABERTO;
  } else { // senha incorreta
    tone(buzzer, 350, 100);
    tentativas++;
    timer = millis();
    estadoAtual = ALERTA;
  }
}



// função caso botão reset seja pressionado
void reset() {
  primeiro_digito = 0;
  segundo_digito = 0;
  terceiro_digito = 0;
  quarto_digito = 0;
}



// implementação da senha mudando: incrementando dígitos respectivos aos botões
void incrementarPrimeiro() {
  primeiro_digito++;
  if (primeiro_digito == 10) primeiro_digito = 0;
}
void incrementarSegundo() {
  segundo_digito++;
  if (segundo_digito == 10) segundo_digito = 0;
}
void incrementarTerceiro() {
  terceiro_digito++;
  if (terceiro_digito == 10) terceiro_digito = 0;
}
void incrementarQuarto() {
  quarto_digito++;
  if (quarto_digito == 10) quarto_digito = 0;
}
