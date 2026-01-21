#!/usr/bin/env python3
# filepath: /Users/charliemathot/Desktop/webserv/cgi/generator.py

import os
import sys
import time

def generate_html_response(data):
    #time.sleep(6)
    """Generate an HTML response with the provided data"""
    html = """
    <!DOCTYPE html>
    <html>
    <head>
        <title>CGI Response</title>
    </head>
    <body>
        <h1>CGI Script Output</h1>
        <h2>Environment Variables:</h2>
        <ul>
            <li>REQUEST_METHOD: {method}</li>
            <li>QUERY_STRING: {query}</li>
        </ul>
        <h2>Data Received:</h2>
        <pre>{data}</pre>
    </body>
    </html>
    """.format(
        method=os.environ.get('REQUEST_METHOD', 'UNKNOWN'),
        query=os.environ.get('QUERY_STRING', 'NONE'),
        data=data
    )
    return html

# Main handler
def main():
    # Get request method from environment
    method = os.environ.get('REQUEST_METHOD', 'GET')
    
    if method == 'GET':
        # For GET, we use the query string
        data = os.environ.get('QUERY_STRING', '')
    elif method == 'POST':
        # For POST, read from stdin (body data)
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        data = sys.stdin.read(content_length) if content_length > 0 else ''
    else:
        data = f"Unsupported method: {method}"
    
    # Generate the HTML response
    response = generate_html_response(data)
    
    # Print the response (this will go to the output pipe)
    print("Content-type: text/html\r")  # HTTP header
    print("\r")                         # Blank line to separate headers from body
    print(response)                     # Response body

if __name__ == '__main__':
    main()