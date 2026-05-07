# cofre-arduino

fsm firmware: sempre está em algum estado (trancado, senha, aberto, alerta)
6 botoes: 1 a 4 e enter e reset
2 leds: 1 verde qnd abrir 1 vermelho qnd estiver fechado. se erra vermlho pisca 3 vezes


documentacao completa:

senha: 1326 (qtde de vezes que o botao é pressionado 3 = 3x segundo botao)
10segundos sem interacao -> volta ao trancado
caso erre 3x, o led vermelho pisca por 10sec , mensagem visor "BLOEUQOADO"e dps desbloqueia

sensor de luz: NAO ABRE o cofre no escuro! (bloqueado ate ter luz)
botões: primerio | segundo | terceiero | quarto | enter | reset


diagrama de conexao: desenho de como tudo se conecta, +, -, portas etc
