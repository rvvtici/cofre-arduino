# cofre-arduino

## foto tinkercad

## Objetivo
Criação de um locker inteligente com feedback multissensorial, implementando um sistema de controle de acesso para um compartimeneto seguro. O sistema deve gerenciar a entrada de uma senha, validar o acesso e monitorar condiçõe s de segurança do ambiente.
- Integração de múltiplos periféricos: LCD, potenciômetro, servomotor, buzzer, LEDs, botões, fotossensor;
- Implementação de lógica de estados: Sistema baseado em Máquina de Estados Finita (FSM) com estados TRANCADO, SENHA, ABERTO, ALERTA, BLOQUEADO;
- Manipuação de sinais analógicos e digitais: processamento de entradas variadas para funções críticas de segurança ou reset;
- Desenvolvimento de interface de usuário: criação de mensagens dinâmicas no LCD que orientem o usuário de forma clara e intuitiva.

## Componentes Utilizados
- Arduino Uno;
- Protoboard;
- LCD;
- Potenciômetro;
- Botões;
- LEDs;
- Buzzer;
- Fotossensor;
- Servomotor;

## Funcionalidades
- LCD serve como display para orientar o usuário em diferentes estados: TRANCADO, SENHAS, ABERTO, ALERTA, BLOQUEADO;
- Botões estão na ordem: primeiro_dígito | segundo_dígito | terceiro_dígito | reset | quarto_dígito | enter;
- A senha é de quatro dígitos e deve ser inserida a partir dos botões;

## Fluxo (desenho)


## Estados
- TRANCADO: Estado inicial do cofre, display "TRANCADO";
- SENHA: Quando qualquer botão é pressionado o estado é ativado e o display "SENHA: 0000" é ativo e responsivo às entradas do usuário;
- ALERTA: Caso a senha tenha sido inserida incorretamente, mostra o estado "SENHA INCORRETA: X/3 tentativas" e o led vermelho pisca. Se incorreta quatro vezes, vai para o estado bloqueado;
- BLOQUEADO: Display "BLOQUEADO", led vermelho ligado e um countdown de 10 segundos para voltar ao estado TRANCADO;
- ABERTO: Servomotor abre, led verde liga e após dez segundos de sua abertura começa a detectar a luz do ambiente com o fotossensor. Caso a luz esteja baixa, o servomotor fecha.


## Senha
1326

## Diagrama de Conexão:
desenho de como tudo se conecta, +, -, portas etc
