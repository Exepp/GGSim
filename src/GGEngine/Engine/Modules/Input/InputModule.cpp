#include <GGEngine/Engine/Modules/Input/InputModule.h>
#include <GGEngine/Game.h>

void setInputDivice(RawKey& key, const sf::Event& event);

void setKeyState(RawKey& key, const sf::Event& event);

void InputModule::update()
{
    currentInput.resetKeys();

    assignInputKeys();
    assignMousePosition();
}

void InputModule::assignMousePosition()
{
    auto& windowModule = Game::Modules.window;
    auto& window = windowModule.getWin();

    Vec2f newAbsPos = asVec<Vec2f>(sf::Mouse::getPosition(window)).clamp(Vec2f::ZeroVector, asVec<Vec2f>(window.getSize()));

    currentInput.cursorDeltaPos = newAbsPos - currentInput.absCursorPos;
    currentInput.absCursorPos = newAbsPos;
    currentInput.signedCursorPos = Vec2f(currentInput.absCursorPos - asVec<Vec2f>(window.getSize()) / 2.f).clamp(-asVec<Vec2f>(window.getSize()) / 2.f, asVec<Vec2f>(window.getSize()) / 2.f);


    scaleVectorByVector(currentInput.cursorDeltaPos, windowModule.getViewToWindowRatio());
    scaleVectorByVector(currentInput.absCursorPos, windowModule.getViewToWindowRatio());
    scaleVectorByVector(currentInput.signedCursorPos, windowModule.getViewToWindowRatio());
}

void InputModule::assignInputKeys()
{
    auto& windowModule = Game::Modules.window;
    auto& window = windowModule.getWin();

    RawKey key;
    sf::Event event;
    uint8_t iCount = 0;

    while (window.pollEvent(event)) {
        setInputDivice(key, event);
        if (key.divice != InputDivice::Count) {
            key.code = (RawKeyCode)event.key.code;
            setKeyState(key, event);

            currentInput.rawKeys[iCount++] = key;
        }
        // for test purpose (deallocation)
        else if (event.type == sf::Event::EventType::Closed)
            window.close();
        ///////////////////////////////////////////////////////////////////////////
    }
}

void setInputDivice(RawKey& key, const sf::Event& event)
{
    if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::KeyReleased)
        key.divice = InputDivice::Keyboard;
    else if (event.type == sf::Event::EventType::MouseButtonPressed || event.type == sf::Event::EventType::MouseButtonReleased)
        key.divice = InputDivice::Mouse;
    else
        key.divice = InputDivice::Count;
}

void setKeyState(RawKey& key, const sf::Event& event)
{
    if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed)
        key.state = KeyState::Pressed;
    else if (event.type == sf::Event::EventType::KeyReleased || event.type == sf::Event::EventType::MouseButtonReleased)
        key.state = KeyState::Released;
}

const RawInputPack& InputModule::getCurrentInput() const
{
    return currentInput;
}
