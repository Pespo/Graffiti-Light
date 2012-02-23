#include "Program.hpp"
#include <iostream>

using namespace std;

Program* Program::s_pCurrentProgram;

Program::Program(const string& vertexPath, const string& fragmentPath) {
    cout << "new Program" << endl;

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER),
         fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    cout << "Program : shaders created" << endl;

	const char*   vertexSource = loadFromFile(vertexPath);
    const char* fragmentSource = loadFromFile(fragmentPath);
    cout << "Program : shaders loaded" << endl;
    
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
    cout << "Program : vertex shaders source ok" << endl;
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    cout << "Program : vertex shaders source ok" << endl;
    
	delete vertexSource;
    delete fragmentSource;
    cout << "Program : delete shaders source ok" << endl;

    GLuint shaders[2] = {vertexShader, fragmentShader};
    
    GLint compiled;
    for (int i = 0; i < 2; ++i) {
        glCompileShader(shaders[i]);
        glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &compiled); 
        if(compiled != GL_TRUE){
            GLint length; GLchar *buffer;
            glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &length);
            buffer = new char[length];
            glGetShaderInfoLog( shaders[i], length, NULL, buffer);
            cerr << buffer << endl;
            cerr << endl << "-------------------------------------" << endl;
            delete[] buffer;
        }
    }
    
    m_glId = glCreateProgram();
	glAttachShader(m_glId, fragmentShader);
	glAttachShader(m_glId, vertexShader);
	glLinkProgram(m_glId);
    cout << "Program : linked" << endl;
    
    GLint linked;
    glGetProgramiv(m_glId, GL_LINK_STATUS, &linked);
    if(linked != GL_TRUE) {
        GLint length; char *buffer;
        glGetProgramiv(m_glId, GL_INFO_LOG_LENGTH, &length);
        buffer = new char[length];
        glGetProgramInfoLog(m_glId, length, NULL, buffer);
        cerr << buffer << endl << "-------------------------------------" << endl;
        delete[] buffer;
    }
}

const char* Program::loadFromFile(const string& fn) const {
    cout << "Program : load from file" << endl;
	FILE *fp;
	char *content = NULL;
	int count = 0;
    
	if (!fn.empty()) {
        cout << "Program : open file" << endl;

		fp = fopen(fn.c_str(), "rt");
        
		if (fp != NULL) {
            cout << "Program : file unreachable" << endl;

            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
    cout << "Program : size of file = " << count << endl;
	return content;
}

Program::~Program() {
    glDeleteProgram(m_glId);
    //delete s_pCurrentProgram;
}

void Program::active() {
    s_pCurrentProgram = this;
    glUseProgram(m_glId);
}

void Program::deactive() {
    s_pCurrentProgram = NULL;
    glUseProgram(0);
}

void Program::setTexture(const std::string& name, const size_t& value) const {
    glUniform1i(glGetUniformLocation(m_glId, name.c_str()), value);
}

void Program::setFloat(const std::string& name, const float& value) const {
    glUniform1f(glGetUniformLocation(m_glId, name.c_str()), value);
}

const Program* Program::getCurrent() {
    return s_pCurrentProgram;
}