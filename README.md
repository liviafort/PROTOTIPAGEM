# Sistema de Automação Residencial com ESP32 e Alexa

## Descrição

Este projeto propõe o desenvolvimento de um sistema de automação doméstica para o controle de dispositivos elétricos utilizando um módulo relé e um microcontrolador ESP32. O sistema integra-se com a assistente virtual Alexa, permitindo o controle de dispositivos como ventiladores, lâmpadas, e outros aparelhos eletrônicos, de forma simples e eficiente. 

O backend foi desenvolvido para proporcionar a flexibilidade de atualização dinâmica das credenciais de Wi-Fi e configurações dos dispositivos via requisições HTTP. Além disso, a integração com a plataforma Sinric Pro facilita a comunicação entre o ESP32 e a Alexa, sem necessidade de programação adicional no microcontrolador.

## Funcionalidades

- **Controle de dispositivos**: O sistema permite controlar dispositivos conectados ao relé por comandos da Alexa, como ligar/desligar.
- **Atualização de credenciais e configurações**: O backend oferece a possibilidade de atualizar as configurações e credenciais de Wi-Fi em tempo real.
- **Escalabilidade**: O backend permite controlar não só ventiladores, mas outros dispositivos eletrônicos conectados ao sistema.
- **Integração com Alexa**: Utilizando o Sinric Pro, o ESP32 é facilmente integrado à Alexa, permitindo o controle dos dispositivos diretamente pelo aplicativo da assistente virtual.
- **Sistema simples e acessível**: A solução foi desenvolvida com foco na simplicidade e baixo custo, aproveitando o potencial da Internet das Coisas (IoT).

## Arquitetura do Sistema

1. **ESP32**: Microcontrolador responsável por controlar os relés que acionam os dispositivos eletrônicos, conectando-se à rede Wi-Fi local.
2. **Relé**: Módulo utilizado para acionar ou desligar os dispositivos elétricos conectados.
3. **Alexa**: Assistente virtual que envia comandos para o ESP32, acionando os relés conforme necessário.
4. **Backend**: Desenvolvido em NestJS com TypeORM, permite a atualização dinâmica de configurações e credenciais de dispositivos via HTTP.
5. **Sinric Pro**: Plataforma utilizada para integrar o ESP32 com a Alexa, permitindo o controle dos dispositivos diretamente no aplicativo da Alexa.

## Instalação e Configuração

### 1. Backend
O backend do projeto está hospedado no Railway e pode ser acessado através da URL:

[http://ota-esp32-production.up.railway.app](http://ota-esp32-production.up.railway.app)

O backend permite atualizar as credenciais de Wi-Fi e configurações dos dispositivos. Certifique-se de que as configurações estão corretamente definidas no sistema para que o ESP32 possa se conectar corretamente à rede.

### 2. Configuração do ESP32

- Conecte o módulo relé ao ESP32 e aos dispositivos que você deseja controlar.
- Certifique-se de que o ESP32 está configurado para se conectar ao backend e receber comandos via HTTP.
- Integre o ESP32 com a plataforma Sinric Pro para permitir o controle pela Alexa.

### 3. Controle via Alexa

- Vincule o ESP32 à sua conta da Alexa através da plataforma Sinric Pro.
- No aplicativo Alexa, você verá os dispositivos conectados e poderá controlá-los diretamente.
