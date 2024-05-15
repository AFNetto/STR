Simulação de cruzamento de trilhos de trem

O projeto envolve criar um sistema que simule o funcionamento de um cruzamento entre uma linha ferroviária e uma via de carros. Neste cenário, os trens e os carros podem se aproximar de qualquer das direções. Para evitar colisões, sempre que um trem se aproxima do cruzamento, uma cancela deve ser fechada e os veículos devem parar. A sincronização do acesso ao cruzamento é crucial e será gerenciada usando semáforos. Você deverá aplicar os conceitos de sincronização e exclusão mútua.
* A implementação deverá ser feita usado o FreeRTOS.
Requisitos

    -> Modelagem dos trens e carros: cada trem e cada carro deve ser representado por uma thread (ou processo). Eles podem vir de qualquer uma das quatro direções. O importante é que em nenhum momento exista mais de um trem no cruzamento e no mesmo sentido no mesmo instante.
    -> Cruzamento: Deve ser tratado como um recurso compartilhado, onde o acesso é controlado por semáforos. Note que podemos ter dois trens cruzando se eles estiverem em sentidos opostos. Os carros só podem cruzar se a cancela não estiver abaixada e a passagem do trem sempre tem prioridade.
    -> Semáforos: utilizar semáforos para gerenciar o acesso ao cruzamento. Deve-se garantir que os semáforos sejam usados para evitar condições de corrida e deadlocks, permitindo uma passagem segura e eficiente.
    -> Interface com o usuário: uma interface simples que mostre o estado atual dos trens e dos carros (aproximando-se, passando, e passou) e a situação do cruzamento. Pode ser um simples output de texto ou uma interface gráfica básica.
