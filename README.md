# cofre-arduino

## Implementação no TinkerCad
<img width="500" height="300" alt="image" src="https://github.com/user-attachments/assets/d32c20fd-70a8-41cf-9796-9193f489a750" />


## Objetivo
Criação de um locker inteligente com feedback multissensorial, implementando um sistema de controle de acesso para um compartimeneto seguro. O sistema deve gerenciar a entrada de uma senha, validar o acesso e monitorar condiçõe s de segurança do ambiente.
- Integração de múltiplos periféricos: LCD, potenciômetro, servomotor, buzzer, LEDs, botões, fotossensor;
- Implementação de lógica de estados: Sistema baseado em Máquina de Estados Finita (FSM) com estados TRANCADO, SENHA, ABERTO, ALERTA, BLOQUEADO;
- Manipulação de sinais analógicos e digitais: processamento de entradas variadas para funções críticas de segurança ou reset;
- Desenvolvimento de interface de usuário: criação de mensagens dinâmicas no LCD que orientem o usuário de forma clara e intuitiva.

## Como Rodar Fisicamente
- Instale o software do Arduino em: https://www.arduino.cc/en/software/
- Conecte todos os cabos e periféricos de acordo com a foto: [[foto tinkercad]]
- Rode e siga as [[instruções]] abaixo
  
## Componentes Utilizados
- Arduino Uno;
- Protoboard;
- LCD;
- Potenciômetro;
- Botões;
- LEDs;
- Buzzer;
- Fotossensor;
- Servomotor.

## Funcionalidades
- LCD serve como display para orientar o usuário em diferentes estados: TRANCADO, SENHAS, ABERTO, ALERTA, BLOQUEADO;
- Botões estão na ordem: primeiro_dígito | segundo_dígito | terceiro_dígito | reset | quarto_dígito | enter;
- A senha (1326) é de quatro dígitos e deve ser inserida a partir dos quatro botões de dígito;

## Fluxo (desenho em fluxograma)


## Estados
- TRANCADO: Estado inicial do cofre. Mostra "TRANCADO" em seu display e mantém o led vermelho aceso;
- SENHA: Estado ativado caso qualquer botão seja pressionado e o estado anterior seja TRANCADO. Exibe display "SENHA: 0000" e é responsivo às entradas do usuário;
- ALERTA: Estado ativado caso a SENHA esteja incorreta. No display: "SENHA INCORRETA: x/3", sendo x a quantidade de tentativas incorretas preenchidas pelo usuário. Aleḿ disso, o led vermelho pisca e o buzzer é ativado por 1s;
- BLOQUEADO: Ocorre caso SENHA tenha quatro tentativas incorretas. Exibe "BLOQUEADO 10s" no display com um countdown até zero segundos, além de ligar o led vermelho. Após a contagem, retorna ao estado TRANCADO;
- ABERTO: Caso a senha correta (1326) seja preenchida, o cofre (servomotor) abre, o led verde liga. Após dez segundos de abertura, começa a detectar a luz do ambiente com o fotossensor. Caso a luz esteja baixa, o cofre (servomotor) fecha.

## Comentários em Relação ao Código
```
teste
```

## Diagrama de Conexão:
desenho de como tudo se conecta, +, -, portas etc (schematic view do tinkercad)
<img width="600" height="450" alt="image" src="https://github.com/user-attachments/assets/6ed68509-2700-4dfa-bdd6-8cd2a5898098" />


