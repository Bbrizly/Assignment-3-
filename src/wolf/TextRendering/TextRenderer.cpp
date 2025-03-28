#include "TextRenderer.h"
using namespace std;

TextRenderer::TextRenderer()
    : TextShader(nullptr) {}

TextRenderer::~TextRenderer() {
    if (TextShader != nullptr) {
        wolf::ProgramManager::DestroyProgram(TextShader);
    }
    if(m_textBoxes.size() > 0)
        m_textBoxes.clear();

}

void TextRenderer::init(TextTable* pTable) {
    init();
    m_pTextTable = pTable;
}
void TextRenderer::init() {
    TextShader = wolf::ProgramManager::CreateProgram("data/2d.vsh", "data/2d.fsh");
}

void TextRenderer::update(float dt) {}

void TextRenderer::setTextBox(TextBox* textBox) {
    m_textBoxes.push_back(textBox);}

void TextRenderer::render(const glm::mat4& proj, const glm::mat4& view) {

    if (!TextShader || m_textBoxes.empty()) {
        std::cout << "TextShader is NULL or no text boxes available." << std::endl;
        return;
    }

    glm::mat4 orthoProj = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f); 
    
    //1280 by 720 divided by 2 to get these numbs 
    // glm::mat4 uiProjection = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 1.0f); //CHANGE ALTER cuz depends on aspect ratio, UI might freak out

    glm::mat4 uiProjection = glm::ortho(
        -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
        -m_screenHeight / 2.0f, m_screenHeight / 2.0f,
        -1.0f, 1.0f
    );

    glm::mat4 identityView = glm::mat4(1.0f);

    for( auto textBox : m_textBoxes ){
        textBox->Render(uiProjection, identityView);
    }
}

Font* TextRenderer::createFont(const string& fontDataPath) {
    return new Font(fontDataPath);
}

TextBox* TextRenderer::createTextBox(Font* font, const string& text, float x, float y, float width, float height) {
    auto textBox = new TextBox(font, text, width, height, x, y, TextShader, m_pTextTable);
    return textBox;
}