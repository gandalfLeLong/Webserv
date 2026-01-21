#include "Response.hpp"

static std::string to_string(int number)
{
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

static bool ends_with(std::string str, std::string suffix)
{
	if (str.length() < suffix.length())
		return (false);
	return (str.rfind(suffix) == (str.length() - suffix.length()));
}

Response::Response(Request request)
{
	this->body = "";
	this->httpVersion = request.getVersion();
	this->statusCode = request.getError();
	if (this->statusCode == 0)
		this->statusCode = 200;
	this->StatusMess = request.getErrorMessage();
	if (this->StatusMess == "")
		this->StatusMess = "OK";
}

Response::~Response()
{

}

void	Response::addHeader(std::string name, std::string value)
{
	this->headers[name] = value;
}

void	Response::pathHandler(std::string path)
{
	if (ends_with(path, ".html"))
		addHeader("Content-Type", "text/html");
	else if (ends_with(path, ".css"))
		addHeader("Content-Type", "text/css");
	else if (ends_with(path, ".js"))
		addHeader("Content-Type", "application/javascript");
	else if (ends_with(path, ".json"))
		addHeader("Content-Type", "application/json");
	else if (ends_with(path, ".png"))
		addHeader("Content-Type", "image/png");
	else if (ends_with(path, ".jpg") || ends_with(path, ".jpeg"))
		addHeader("Content-Type", "image/jpeg");
	else if (ends_with(path, ".gif"))
		addHeader("Content-Type", "image/gif");
	else if (ends_with(path, ".txt"))
		addHeader("Content-Type", "text/plain");
	else if (ends_with(path, ".pdf"))
		addHeader("Content-Type", "application/pdf");
	else if (ends_with(path, "/submit"))
		addHeader("Content-Type", "text/plain");
	else if (ends_with(path, "/upload"))
		addHeader("Content-Type", "text/plain");
	else if (ends_with(path, "/xml"))
		addHeader("Content-Type", "application/xml");
	else if (ends_with(path, ".csv"))
		addHeader("Content-Type", "text/csv");
	else if (ends_with(path, ".mp3"))
		addHeader("Content-Type", "audio/mpeg");
	else if (ends_with(path, ".mp4"))
		addHeader("Content-Type", "video/mp4");
	else if (ends_with(path, ".woff"))
		addHeader("Content-Type", "font/woff");
	else if (ends_with(path, "woff2"))
		addHeader("Content-Type", "font/woff2");
	else if (ends_with(path, ".ttf"))
		addHeader("Content-Type", "font/ttf");
	else if (ends_with(path, ".zip"))
		addHeader("Content-Type", "application/zip");
	else
		addHeader("Content-Type", "application/octet-stream");
}

void	Response::addBodyHeader(std::string body)
{
	this->body = body;
	addHeader("Content-Length", to_string(body.size()));
}

std::string	Response::generateResponse()
{
	std::string	response;

	response = this->httpVersion + " " + to_string(this->statusCode) + " " + this->StatusMess + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = this->headers.begin(); it != this->headers.end(); ++it)
		if (it->first != "Connection" && it->first != "Content-Length" && it->first != "Content-Type")
			response += it->first + ": " + it->second + "\r\n";
	if (this->headers.find("Content-Type") != this->headers.end())
		response += "Content-Type: " + this->headers["Content-Type"] + "\r\n";
	if (this->headers.find("Content-Length") != this->headers.end())
		response += "Content-Length: " + this->headers["Content-Length"] + "\r\n";
	if (this->headers.find("Connection") != this->headers.end())
		response += "Connection: " + this->headers["Connection"] + "\r\n";
	response += "\r\n";
	if (this->body.size() >= 1)
		response += this->body + "\r\n";
	return (response);
}

void	Response::output(void)
{
	std::cout << "output Response" << std::endl;
}