#include "Program.hpp"
#include "OpenGL.hpp"
#include <iostream>

using namespace std;

Program* Program::s_pCurrentProgram;

Program::Program(const string& vertexPath, const string& fragmentPath) {
    cout << "Program : new" << endl;

	GLuint   vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexSource = loadFromFile(vertexPath);
    const char* fragmentSource = loadFromFile(fragmentPath);
    
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	delete vertexSource;
    delete fragmentSource;

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
		fp = fopen(fn.c_str(), "rt");
        
		if (fp != NULL) {
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
	return content;
}

Program::~Program() {
    glDeleteProgram(m_glId);
    //delete s_pCurrentProgram;
}

void Program::active() {
    cout << "Program : active " << m_glId << endl;
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