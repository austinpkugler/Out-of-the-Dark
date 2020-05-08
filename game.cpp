#include "game.h"


// default constructor for Game class
// no return
// no parameters
Game::Game(){}

Game::~Game(){}

// constructor for Game class
// no return
// no parameters
Game::Game(sf::RenderWindow* window)
{
    m_screenName = "title_screen";
    m_window = window;
    m_width = m_window->getSize().x;
    m_height = m_window->getSize().y;
    loadSettings();
    load();
    m_backgroundSprite.setScale(m_window->getSize().x / m_backgroundSprite.getLocalBounds().width,
                                m_window->getSize().y / m_backgroundSprite.getLocalBounds().height);
    getAllSaves();
}

/**
 * @brief Manages the loading of all game assets.
 * @details Loads global assets and then calls specific load functions
 * based on the current game state.
 * @throw SFML exceptions are thrown when assets fail to load. The program may
 * terminate when fatal errors occur.
 * @param None
 * @return None
 */
void Game::load()
{
    if (!m_soundBuffer.loadFromFile("assets/clicked.wav"))
    {
        std::cout << "Game: Failed to load asset clicked.wav\n";
        std::exit(1);
    }
    if (!m_font.loadFromFile("assets/rm_typerighter.ttf"))
    {
        std::cout << "Game: Failed to load asset rm_typerighter.ttf\n";
        std::exit(1);
    }

    if (m_screenName == "title_screen")
    {
        std::cout << "Game: Loading title screen\n";
        titleScreenLoad();
    }
    else if (m_screenName == "play_screen")
    {
        std::cout << "Game: Loading play screen\n";
        playScreenLoad();
    }
    else if (m_screenName == "settings_screen")
    {
        std::cout << "Game: Loading settings screen\n";
        settingsScreenLoad();
    }
    else if (m_screenName == "settings_screen")
    {
        std::cout << "Game: Loading settings screen\n";
        settingsScreenLoad();
    }
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::handleInput()
{
    if (m_screenName == "title_screen")
    {
        titleScreenInput();
    }
    else if (m_screenName == "play_screen")
    {
        playScreenInput();
    }
    else if (m_screenName == "settings_screen")
    {
        settingsScreenInput();
    }
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::update()
{
    if (m_screenName == "title_screen")
    {
        titleScreenUpdate();
    }
    else if (m_screenName == "play_screen")
    {
        playScreenUpdate();
    }
    else if (m_screenName == "settings_screen")
    {
        settingsScreenUpdate();
    }
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::render()
{
    renderBackground();

    if (m_screenName == "settings_screen")
    {
        renderSettings();
    }

    if (m_settings->showFps && m_frameCount % (m_settings->frameRate / 4) == 0)
    {
        m_displayedFps = m_fps;
        if (m_displayedFps > m_settings->frameRate)
        {
            m_displayedFps = m_settings->frameRate;
        }
    }
    if (m_settings->showFps)
    {
        sf::Text text;
        
        text.setFont(m_font);
        char c[10];
        sprintf(c, "%i", m_displayedFps);
        text.setString(c);
        text.setPosition(15, 0);
        m_window->draw(text);
    }
    m_frameCount++;
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::renderBackground()
{
    m_window->draw(m_backgroundSprite);
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::renderSettings()
{
    sf::RectangleShape rectangle(sf::Vector2f(m_width * 0.09, m_height * 0.05));
    rectangle.setFillColor(sf::Color(0, 0, 0, 0));
    rectangle.setOutlineColor(sf::Color(255, 255, 255));
    rectangle.setOutlineThickness(2);
    if (m_settings->playMusic)
    {
        rectangle.setPosition(m_width * 0.3, m_height * 0.25);
        m_window->draw(rectangle);
    }
    else
    {
        rectangle.setPosition(m_width * 0.4, m_height * 0.25);
        m_window->draw(rectangle);
    }

    if (m_settings->playAudio)
    {
        rectangle.setPosition(m_width * 0.3, m_height * 0.35);
        m_window->draw(rectangle);
    }
    else
    {
        rectangle.setPosition(m_width * 0.4, m_height * 0.35);
        m_window->draw(rectangle);
    }

    if (m_settings->difficulty)
    {
        rectangle.setPosition(m_width * 0.4, m_height * 0.45);
        m_window->draw(rectangle);
    }
    else
    {
        rectangle.setPosition(m_width * 0.3, m_height * 0.45);
        m_window->draw(rectangle);
    }

    if (m_settings->frameRate == 30)
    {
        rectangle.setPosition(m_width * 0.3, m_height * 0.55);
        m_window->draw(rectangle);
    }
    else if (m_settings->frameRate == 60)
    {
        rectangle.setPosition(m_width * 0.4, m_height * 0.55);
        m_window->draw(rectangle);
    }
    else if (m_settings->frameRate == 120)
    {
        rectangle.setPosition(m_width * 0.5, m_height * 0.55);
        m_window->draw(rectangle);
    }

    if (m_settings->showFps)
    {
        rectangle.setPosition(m_width * 0.3, m_height * 0.65);
        m_window->draw(rectangle);
    }
    else
    {
        rectangle.setPosition(m_width * 0.4, m_height * 0.65);
        m_window->draw(rectangle);
    }
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::clearScreen()
{
    m_window->clear(sf::Color::Black);
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
int Game::getFps() const
{
    return m_fps;
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::setFps(unsigned int fps)
{
    m_fps = fps;
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::playSoundBuffer()
{
    if (m_settings->playAudio)
    {
        sf::Sound sound;
        sound.setBuffer(m_soundBuffer);
        sound.play();
        std::cout << "Audio: Playing sound effect from buffer\n";
    }
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
bool Game::isDone() const
{
    return !(m_window->isOpen());
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::loadSettings()
{
    std::fstream file("user_data/settings.csv", std::ios::in);
    char comma = ',';
    std::string parameterName;

    file >> parameterName >> m_settings->playMusic;
    file >> parameterName >> m_settings->playAudio;
    file >> parameterName >> m_settings->difficulty;
    file >> parameterName >> m_settings->frameRate;
    file >> parameterName >> m_settings->showFps;
    file.close();
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::updateSettings()
{
    std::fstream file("user_data/settings.csv", std::ios::out);
    const char comma = ',';
    file << "PLAY_MUSIC" << comma << ' ' << m_settings->playMusic << '\n';
    file << "PLAY_AUDIO" << comma << ' ' << m_settings->playAudio << '\n';
    file << "DIFFICULTY" << comma << ' ' << m_settings->difficulty << '\n';
    file << "FRAME_RATE" << comma << ' ' << m_settings->frameRate << '\n';
    file << "SHOW_FPS" << comma << ' ' << m_settings->showFps << '\n';
    m_window->setFramerateLimit(m_settings->frameRate);
    if (!m_settings->playMusic)
    {
        m_music.stop();
        std::cout << "Game: Stopped all music playback\n";
    }
    file.close();
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
std::vector<std::string> Game::getAllSaves()
{
    std::vector<std::string> saveFiles;
    std::fstream file("user_data/one.save", std::ios::in);
    if (!file.fail())
    {
        saveFiles.push_back("one.save");
    }
    file.close();

    file.open("user_data/two.save", std::ios::in);
    if (!file.fail())
    {
        saveFiles.push_back("two.save");
    }
    file.close();

    file.open("user_data/three.save", std::ios::in);
    if (!file.fail())
    {
        saveFiles.push_back("three.save");
    }
    file.close();

    return saveFiles;
}

/*******************************************************************************
 * Functions for titleScreen change this comment to be better
 * 
 * 
 ******************************************************************************/
/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::titleScreenUpdate()
{
    m_backgroundSprite.setTexture(m_titleScreenBg);
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::titleScreenLoad()
{
    if (!m_titleScreenBg.loadFromFile("assets/title_screen_background.png"))
    {
        std::exit(1);
    }
    m_backgroundSprite.setTexture(m_titleScreenBg);
    std::cout << "Game: Title screen loaded\n";

    if (m_settings->playMusic && m_music.getStatus() == sf::Music::Status::Stopped)
    {
        std::cout << "Title Screen: Playing music\n";
        if (!m_music.openFromFile("assets/2nd_Sonata_Malign_Chords.ogg"))
        {
            std::exit(1);
        }
        m_music.setVolume(20.f);
        m_music.setLoop(true);
        m_music.play();
    }
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::titleScreenInput()
{
    sf::Event event;
    while(m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)    // Close window button clicked.
        {
            m_window->close();
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                float width = m_window->getSize().x;
                float height = m_window->getSize().y;
                if (event.mouseButton.x >= width * 0.10 &&
                    event.mouseButton.x <= width * 0.25)
                {
                    if (event.mouseButton.y >= height * 0.25 &&
                        event.mouseButton.y <= height * 0.3)
                    {
                        std::cout << "Game: 'Play Game' button pressed\n";
                        m_screenName = "play_screen";
                        load();
                    }
                    else if (event.mouseButton.y >= height * 0.35 &&
                             event.mouseButton.y <= height * 0.4)
                    {
                        std::cout << "Game: 'Settings' button pressed\n";
                        m_screenName = "settings_screen";
                        load();
                    }
                    else if (event.mouseButton.y >= height * 0.45 &&
                             event.mouseButton.y <= height * 0.5)
                    {
                        std::cout << "Game: 'Quit' button pressed\n";
                        m_window->close();
                    }
                }
            }
        }
    }
}

/*******************************************************************************
 * Functions for playScreen change this comment to be better
 * 
 * 
 ******************************************************************************/
/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::playScreenInput()
{
    sf::Event event;
    while(m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)    // Close window button clicked.
        {
            m_window->close();
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                float width = m_window->getSize().x;
                float height = m_window->getSize().y;
                if (event.mouseButton.y >= height * 0.23 &&
                    event.mouseButton.y <= height * 0.69)
                {
                    if (event.mouseButton.x >= width * 0.09 &&
                        event.mouseButton.x <= width * 0.31)
                    {
                        std::cout << "Save Slot 1 has been pressed\n";
                    }
                    else if (event.mouseButton.x >= width * 0.39 &&
                             event.mouseButton.x <= width * 0.61)
                    {
                        std::cout << "Save slot 2 has been pressed\n";
                    }
                    else if (event.mouseButton.x >= width * 0.69 &&
                             event.mouseButton.x <= width * 0.91)
                    {
                        std::cout << "Save slot 3 has been pressed\n";
                    }
                }
                if (event.mouseButton.y >= height * 0.75 &&
                    event.mouseButton.y <= height * 0.8)
                {
                    if (event.mouseButton.x >= width * 0.1 &&
                    event.mouseButton.x <= width * 0.165)
                    {
                        m_screenName = "title_screen";
                        load();
                    }
                }
            }
        }
    }
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::playScreenLoad()
{
    if (!m_titleScreenBg.loadFromFile("assets/play_screen_background.png"))
    {
        std::exit(1);
    }
    m_backgroundSprite.setTexture(m_titleScreenBg);
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::playScreenUpdate()
{
    m_backgroundSprite.setTexture(m_titleScreenBg);
}


/*******************************************************************************
 * Functions for settingsScreen change this comment to be better
 * 
 * 
 ******************************************************************************/
/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::settingsScreenUpdate()
{
    m_backgroundSprite.setTexture(m_settingsScreenBg);
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::settingsScreenLoad()
{
    if (!m_settingsScreenBg.loadFromFile("assets/settings_screen_background.png"))
    {
        std::exit(1);
    }

    m_backgroundSprite.setTexture(m_settingsScreenBg);
    std::cout << "Game: Settings screen loaded\n";
}

/**
 * @brief
 * @details
 * @throw
 * @param
 * @return
 */
void Game::settingsScreenInput()
{
    sf::Event event;
    while(m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)    // Close window button clicked.
        {
            m_window->close();
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                float width = m_window->getSize().x;
                float height = m_window->getSize().y;
                if (event.mouseButton.x >= width * 0.30 &&
                    event.mouseButton.x <= width * 0.37)
                {
                    if (event.mouseButton.y >= height * 0.25 &&
                        event.mouseButton.y <= height * 0.30)
                    {
                        m_settings->playMusic = true;
                    }
                    else if (event.mouseButton.y >= height * 0.35 &&
                             event.mouseButton.y <= height * 0.40)
                    {
                        m_settings->playAudio = true;
                    }
                    else if (event.mouseButton.y >= height * 0.45 &&
                             event.mouseButton.y <= height * 0.50)
                    {
                        m_settings->difficulty = 0; // 0 difficulty includes radar, 1 does not
                    }
                    else if (event.mouseButton.y >= height * 0.55 &&
                             event.mouseButton.y <= height * 0.60)
                    {
                        m_settings->frameRate = 30;
                    }
                    else if (event.mouseButton.y >= height * 0.65 &&
                             event.mouseButton.y <= height * 0.70)
                    {
                        m_settings->showFps = true;
                    }
                }
                else if (event.mouseButton.x >= width * 0.40 &&
                         event.mouseButton.x <= width * 0.48)
                {
                    if (event.mouseButton.y >= height * 0.25 &&
                        event.mouseButton.y <= height * 0.30)
                    {
                        m_settings->playMusic = false;
                        std::cout << m_settings->playMusic << '\n';
                    }
                    else if (event.mouseButton.y >= height * 0.35 &&
                             event.mouseButton.y <= height * 0.40)
                    {
                        m_settings->playAudio = false;
                    }
                    else if (event.mouseButton.y >= height * 0.45 &&
                             event.mouseButton.y <= height * 0.50)
                    {
                        m_settings->difficulty = 1; // 0 is easy, 1 is hard
                    }
                    else if (event.mouseButton.y >= height * 0.55 &&
                             event.mouseButton.y <= height * 0.60)
                    {
                        m_settings->frameRate = 60;
                    }
                    else if (event.mouseButton.y >= height * 0.65 &&
                             event.mouseButton.y <= height * 0.70)
                    {
                        m_settings->showFps = false;
                    }
                }
                else if (event.mouseButton.x >= width * 0.50 &&
                         event.mouseButton.x <= width * 0.56)
                {
                    if (event.mouseButton.y >= height * 0.55 &&
                        event.mouseButton.y <= height * 0.6)
                    {
                        m_settings->frameRate = 120;
                    }
                }
                else if (event.mouseButton.x >= width * 0.10 &&
                         event.mouseButton.x <= width * 0.16)
                {
                    if (event.mouseButton.y >= height * 0.75 &&
                        event.mouseButton.y <= height * 0.80)
                    {
                        updateSettings();
                        m_screenName = "title_screen";
                        load();
                    }
                }
            }
        }
    }
}