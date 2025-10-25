#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

//cria o ponto na tela
sf::CircleShape cria_ponto(int x, int y, int width, int heidght){
	sf::CircleShape ponto(width, heidght);
	ponto.setFillColor(sf::Color::White);
	ponto.setOutlineColor(sf::Color::Red);
	ponto.setOutlineThickness(1.0f);
	ponto.setPosition(x,y);
	return ponto;
}
//função que serve ao passar o mouse encima da linha
void passarmouse(sf::RenderWindow &window, sf::CircleShape points[7][7], float x, float y, int dim, int espacamento) {
    sf::Color cor;
    // Primeiro desenha todos os pontos com a cor padrão
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            window.draw(points[i][j]);
        }
    }
    // Calcula a célula e posição relativa para o hover das linhas
    int i = x / (dim + espacamento);
    int j = y / (dim + espacamento);
    float relX = x - i * (dim + espacamento);
    float relY = y - j * (dim + espacamento);

    // Linha horizontal
    if (relX > dim - 15 && relX < dim + 15 && i < 6 && j < 7) {
        sf::RectangleShape linha(sf::Vector2f(espacamento, 6));
        linha.setFillColor(sf::Color::Yellow);
        linha.setPosition(i * (dim + espacamento) + dim, j * (dim + espacamento) + dim / 2);
        window.draw(linha);
    }

    // Linha vertical
    if (relY > dim - 15 && relY < dim + 15 && i < 7 && j < 6) {
        sf::RectangleShape linha(sf::Vector2f(6, espacamento));
        linha.setFillColor(sf::Color::Yellow);
        linha.setPosition(i * (dim + espacamento) + dim / 2, j * (dim + espacamento) + dim);
        window.draw(linha);
    }
}
void desenhalinha(sf::RenderWindow &window, bool matrizHorizontal [6][7], bool matrizVertical[7][6] , int dim, int espacamento){
	for(int i=0; i<6; i++){
		for(int j=0; j<7; j++){
			if(matrizHorizontal[i][j]){
				sf::RectangleShape linha(sf::Vector2f(espacamento, 6));
				linha.setFillColor(sf::Color::Green);
				linha.setPosition(i * (dim + espacamento) + dim, j * (dim + espacamento) + dim/2);
				window.draw(linha);
			}
		}
	}
	for (int i=0;i<7;i++){
		for(int j=0; j<6; j++){
			if(matrizVertical [i][j]){
				sf::RectangleShape linha(sf::Vector2f(6, espacamento));
				linha.setFillColor(sf::Color::Green);
				linha.setPosition(i * (dim + espacamento) + dim/2, j * (dim + espacamento) + dim);
				window.draw(linha);
			}
		}
	}
}
//verifica se o quadrado foi completado e atualiza a pontuação de cada jogador
void verificaQuadrado(bool matrizHorizontal[6][7],bool matrizVertical[7][6],int &pontos_jogador1,int &pontos_jogador2,bool &vez_jogador1,bool quadradosMarcados[6][6], bool quadradosJogador1[6][6]){
	bool quadradoCompleto = false;
    // Verifica todos os quadrados possíveis (6x6)
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            // Verifica se todas as 6 linhas do quadrado estão preenchidas
            bool linhaSuperior = matrizHorizontal[i][j];
            bool linhaInferior = matrizHorizontal[i][j + 1];
            bool linhaEsquerda = matrizVertical[i][j];
            bool linhaDireita = matrizVertical[i + 1][j];
            // Se o quadrado foi completado adiciona 1 ponto ao jogador que ta com a vez
            if (linhaSuperior && linhaInferior && linhaEsquerda && linhaDireita && quadradosMarcados[i][j] == 0){
                if (vez_jogador1) {
                    pontos_jogador1++;
                    quadradosJogador1[i][j] = true;
                }
                else {
                    pontos_jogador2++;
                    quadradosJogador1[i][j] = false;
                }
                quadradoCompleto = true;
                quadradosMarcados[i][j] = true;
            }
        }
    }
    //Se um quadrado foi completado o mesmo jogador joga novamente
    if (!quadradoCompleto) {
        vez_jogador1 = !vez_jogador1; // Troca a vez
    }
}

void desenhaquadrados(sf::RenderWindow &window, int dim, bool quadradosMarcados[6][6], bool quadradosJogador1[6][6], int espacamento) {
	for (int i = 0; i < 6; i++) {
	        for (int j = 0; j < 6; j++) {
	            if (quadradosMarcados[i][j]) {
	                sf::RectangleShape quadrado(sf::Vector2f(espacamento - 10, espacamento - 10));

	                // Usa a vez atual para determinar a cor (isso pode precisar de ajuste)
	                if (quadradosJogador1[i][j]) {
	                    quadrado.setFillColor(sf::Color::Blue); // Azul
	                } else {
	                    quadrado.setFillColor(sf::Color::Red); // Vermelho
	                }

	                quadrado.setPosition(i * (dim + espacamento) + dim + 5,j * (dim + espacamento) + dim + 5);
	                window.draw(quadrado);
	            }
	        }
	    }
}
void desenhaFundoTelaFinal(sf::RenderWindow& window, sf::Texture& fundo) {
    sf::Sprite spriteFundo(fundo);
    window.draw(spriteFundo);
}
void desenhaResultadoFinal(sf::RenderWindow& window, sf::Font& font, int pontos_jogador1, int pontos_jogador2){
	sf::Text textoResultado;
	    textoResultado.setFont(font);
	    textoResultado.setCharacterSize(48);
	    textoResultado.setStyle(sf::Text::Bold);

	    if (pontos_jogador1 > pontos_jogador2) {
	        textoResultado.setString("Jogador 1 Venceu!");
	        textoResultado.setColor(sf::Color::Blue);
	    } else if(pontos_jogador2 > pontos_jogador1) {
	        textoResultado.setString("Jogador 2 Venceu!");
	        textoResultado.setColor(sf::Color::Red);
	    }
	    window.draw(textoResultado);
}
void desenhaInstrucoesReinicio(sf::RenderWindow& window, sf::Font& font) {
    sf::Text instrucao("Pressione R para jogar novamente", font, 24);
    instrucao.setColor(sf::Color::White);
    window.draw(instrucao);
}
void desenhaTelaFinalCompleta(sf::RenderWindow& window, sf::Font& font,sf::Texture& texturaFundo,int pontos_jogador1, int pontos_jogador2) {
    desenhaFundoTelaFinal(window, texturaFundo);
    desenhaResultadoFinal(window, font, pontos_jogador1, pontos_jogador2);
    desenhaInstrucoesReinicio(window, font);
}
bool verificaFimDoJogo(bool matrizHorizontal[6][7], bool matrizVertical[7][6]) {
    // Verifica linhas horizontais
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            if (!matrizHorizontal[i][j]) return false;
        }
    }

    // Verifica linhas verticais
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (!matrizVertical[i][j]) return false;
        }
    }
    return true;
}
void reiniciaJogo(bool matrizHorizontal[6][7], bool matrizVertical[7][6],bool quadradosMarcados[6][6], bool quadradosJogador1[6][6],int &pontos_jogador1, int &pontos_jogador2, bool &vez_jogador1) {
    pontos_jogador1 = 0;
    pontos_jogador2 = 0;
    vez_jogador1 = true;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            matrizHorizontal[i][j] = false;
            if (j < 6) {
                matrizVertical[i][j] = false;
                quadradosMarcados[i][j] = false;
                quadradosJogador1[i][j] = false;
            }
        }
    }
    for (int i = 0; i < 7; i++) {
    	for(int j = 0; j < 6; j++) {
    		matrizVertical[i][j] = false;
    			if (i < 7) {
    				matrizHorizontal[i][j] = false;
    				quadradosMarcados[i][j] = false;
    				quadradosJogador1[i][j] = false;
    			}
    	}
    }
}
//loop de eventos
void trataeventos(sf::RenderWindow *window, bool matrizHorizontal[6][7], bool matrizVertical[7][6],sf::Sound *somdalinha, int dim, int &pontos_jogador1, int &pontos_jogador2,bool &vez_jogador1, bool quadradosMarcados[6][6], bool quadradosJogador1[6][6], bool &jogoAtivo, int espacamento) {
    sf::Event evento;
    while (window->pollEvent(evento)) {
        if (evento.type == sf::Event::Closed)
            window->close();
        if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::R && !jogoAtivo) {
        	reiniciaJogo(matrizHorizontal, matrizVertical, quadradosMarcados, quadradosJogador1,pontos_jogador1, pontos_jogador2, vez_jogador1);
                   jogoAtivo = true;
               }
        if (evento.type == sf::Event::MouseButtonPressed && jogoAtivo) {
            int x = sf::Mouse::getPosition(*window).x;
            int y = sf::Mouse::getPosition(*window).y;

            // Calcula a célula e posição relativa
            int i = x / (dim + espacamento);
            int j = y / (dim + espacamento);
            float relX = x - i * (dim + espacamento);
            float relY = y - j * (dim + espacamento);

            bool linhaAdicionada = false;

            // Verifica clique em linha horizontal (margem de 10 pixels em torno da linha)
            if (relX > dim - 15 && relX < dim + 15 && i < 6 && j < 7) {
                if (!matrizHorizontal[i][j]) {
                    matrizHorizontal[i][j] = true;
                    linhaAdicionada = true;
                }
            }
            // Verifica clique em linha vertical (margem de 10 pixels em torno da linha)
            else if (relY > dim - 15 && relY < dim + 15 && i < 7 && j < 6) {
                if (!matrizVertical[i][j]) {
                    matrizVertical[i][j] = true;
                    linhaAdicionada = true;
                }
            }
            if (linhaAdicionada) {
                somdalinha->play();
                verificaQuadrado(matrizHorizontal, matrizVertical, pontos_jogador1,pontos_jogador2, vez_jogador1, quadradosMarcados,quadradosJogador1);
            }
        }
    }
}
int main() {
	bool jogoAtivo = true;  // Controla se o jogo está em andamento
	int espacamento = 55;
	int dim = 30;
	int pontos_jogador1 = 0;
	int pontos_jogador2 = 0 ;
	bool vez_jogador1 = true; // Controla de quem é a vez
	bool quadradosMarcados[6][6] = {false};
	bool quadradosJogador1[6][6] = {false};  // Marca se foi o jogador 1 que completou

	sf::RenderWindow window(sf::VideoMode(800,600), "Dots and Boxes 2.0", sf::Style::Close | sf::Style::Titlebar );
    window.setFramerateLimit(60); //frame
    window.setVerticalSyncEnabled(true); //sincronização

    sf ::Font font;
    sf::CircleShape points[7][7];
    bool matrizHorizontal[6][7] = {false};
    bool matrizVertical[7][6] = {false};

    for (int i=0; i<7; i++){
    	for(int j=0; j<7; j++){
    		points[i][j] = cria_ponto(i*(dim+espacamento), j*(dim+espacamento), dim/2, dim/2);
    	}
    }
    sf::Texture telafinal;
    telafinal.loadFromFile("assets/fundo.png");

    sf::SoundBuffer somlinha;
    sf::Sound somdalinha;
    somlinha.loadFromFile("assets/drop.ogg");
    somdalinha.setBuffer(somlinha);

    sf::Music music;
    music.openFromFile("assets/vampire.ogg");
    music.play();
    music.setLoop(true);

    font.loadFromFile("assets/x-files.ttf");

    sf::Text TXTpontos_jogador1("Jogador 1:\t0", font, 24);
    TXTpontos_jogador1.setColor(sf::Color::Blue);
    TXTpontos_jogador1.setPosition(560, 40);

    sf::Text TXTpontos_jogador2("Jogador 2:\t0", font, 24);
    TXTpontos_jogador2.setColor(sf::Color::Red);
    TXTpontos_jogador2.setPosition(560, 80);

    while (window.isOpen()) {
    	bool jogoCompleto = verificaFimDoJogo(matrizHorizontal, matrizVertical);
    	jogoAtivo = !jogoCompleto;
    	trataeventos(&window, matrizHorizontal, matrizVertical, &somdalinha, dim,pontos_jogador1, pontos_jogador2, vez_jogador1, quadradosMarcados, quadradosJogador1, jogoAtivo, espacamento);
    	std::ostringstream oss1, oss2;
    	oss1 << "Jogador 1: " << pontos_jogador1;
    	oss2 << "Jogador 2: " << pontos_jogador2;
    	TXTpontos_jogador1.setString(oss1.str());
    	TXTpontos_jogador2.setString(oss2.str());

    	float x = sf::Mouse::getPosition(window).x;
		float y = sf::Mouse::getPosition(window).y;

		window.clear(sf::Color::Black);
		passarmouse(window, points, x, y, dim, espacamento);
		desenhalinha(window, matrizHorizontal,matrizVertical, dim, espacamento);//desenho da linha dps de ser clicado
		desenhaquadrados(window, dim, quadradosMarcados, quadradosJogador1, espacamento);
		window.draw(TXTpontos_jogador1);// Desenha potuação
		window.draw(TXTpontos_jogador2);// Desenha potuação
		if (!jogoAtivo) {
		        desenhaTelaFinalCompleta(window, font, telafinal, pontos_jogador1, pontos_jogador2);
		    }

        window.display();
        sf::sleep(sf::milliseconds(50.0f));
    }
    return 0;
}
