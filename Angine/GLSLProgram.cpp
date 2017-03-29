#include "angine.h"

using namespace Angine;

GLSLProgram::GLSLProgram() : p_programID(0), p_vertexShaderID(0), p_fragmentShaderID(0), p_numAttributes(0) {}

GLSLProgram::~GLSLProgram(){
	Dispose();
}

void GLSLProgram::CompileShadersFromSource(const char* vertexSource, const char* fragmentSource) {
	p_programID = glCreateProgram();

	p_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (p_vertexShaderID == 0)
		throwError("Vertex shader creation failed: ");

	p_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (p_fragmentShaderID == 0)
		throwError("Fragment shader creation failed: ");
	CompileShader(vertexSource, "Vertex shader", p_vertexShaderID);
	CompileShader(fragmentSource, "Fragment shader", p_fragmentShaderID);
}

void GLSLProgram::CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

	std::string fragSource, vertSource;

	IOManager::ReadFileToBuffer(fragSource, fragmentShaderFilePath);
	IOManager::ReadFileToBuffer(vertSource, vertexShaderFilePath);

	CompileShadersFromSource(vertSource.c_str(), fragSource.c_str());
}

void GLSLProgram::CompileShader(const char* source, const std::string& filePath, GLuint shaderID) {

	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(shaderID);

		std::printf("%s\n", &(errorLog[0]));
		throwError("Shader" + filePath + "compilation error: ");
	}
}

void GLSLProgram::LinkShaders() {
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.

	//Attach our shaders to our program
	glAttachShader(p_programID, p_vertexShaderID);
	glAttachShader(p_programID, p_fragmentShaderID);

	//Link our program
	glLinkProgram(p_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(p_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(p_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(p_programID, maxLength, &maxLength, &errorLog[0]);
		std::cout << &(errorLog[0]);
		//We don't need the program anymore.
		glDeleteProgram(p_programID);
		//Don't leak shaders either.
		glDeleteShader(p_vertexShaderID);
		glDeleteShader(p_fragmentShaderID);

		//Use the infoLog as you see fit.
		std::printf("%s\n", &(errorLog[0]));
		throwError("Shader linking error: ");
	}

	//Always detach shaders after a successful link.
	glDetachShader(p_programID, p_vertexShaderID);
	glDetachShader(p_programID, p_fragmentShaderID);
	glDeleteShader(p_vertexShaderID);
	glDeleteShader(p_fragmentShaderID);
}

void GLSLProgram::AddAttribute(const std::string& attributeName) {
	glBindAttribLocation(p_programID, p_numAttributes++, attributeName.c_str());
}

void GLSLProgram::Use() {
	glUseProgram(p_programID);
	for (int i = 0; i < p_numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::UnUse() {
	glUseProgram(0);
	for (int i = 0; i < p_numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::Dispose() {
	if(p_programID)
		glDeleteProgram(p_programID);
}

GLint GLSLProgram::GetUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(p_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		throwError("Uniform " + uniformName + " not found in shader");
	}
	return location;
}
