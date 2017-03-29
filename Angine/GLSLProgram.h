#pragma once

namespace Angine {

	class GLSLProgram {
		GLuint p_programID;
		void CompileShader(const char* source, const std::string& filePath, GLuint shaderID);
		GLuint p_vertexShaderID;
		GLuint p_fragmentShaderID;
		int p_numAttributes;
	public:
		GLSLProgram();
		~GLSLProgram();
		void AddAttribute(const std::string& attriuteName);

		void CompileShadersFromSource(const char* vertexSource, const char* fragmentSource);
		void CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void LinkShaders();

		GLint GetUniformLocation(const std::string& uniformName);

		void Use();
		void UnUse();

		void Dispose();
	};

}