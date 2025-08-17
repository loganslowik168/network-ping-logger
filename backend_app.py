import os
from flask import Flask, send_from_directory, jsonify, request
from flask_cors import CORS
from dotenv import load_dotenv # Import load_dotenv

# Load environment variables from .env file
load_dotenv()

app = Flask(__name__)
CORS(app) # Enable CORS for all routes

# Define the path to your log file relative to this script
LOG_FILE_NAME = "ping_log.txt"

# --- FIX: Retrieve credentials from environment variables ---
# This makes it easier to change credentials without modifying code.
# For production, ensure these environment variables are set securely in your deployment environment.
VALID_USERNAME = os.getenv("APP_USERNAME", "admin") # Default to 'admin' if not found in .env/env
VALID_PASSWORD = os.getenv("APP_PASSWORD", "password123") # Default to 'password123' if not found in .env/env
# --- End FIX ---

@app.route('/')
def serve_index():
    # Serve the index.html file from the current directory
    return send_from_directory(os.getcwd(), 'index.html')

@app.route('/config.js')
def serve_config():
    # Serve the config.js file
    return send_from_directory(os.getcwd(), 'config.js')

@app.route('/get_log_data')
def get_log_data():
    # Basic authentication check (for demonstration purposes only!)
    auth_header = request.headers.get('Authorization')
    if not auth_header:
        return jsonify({"error": "Authentication required"}), 401

    try:
        auth_type, credentials = auth_header.split(None, 1)
        if auth_type.lower() == 'basic':
            # Decode base64 credentials (username:password)
            import base64
            decoded_credentials = base64.b64decode(credentials).decode('utf-8')
            username, password = decoded_credentials.split(':', 1)

            # Debugging print (you can remove this once it works)
            print(f"Attempted Login: Username='{username}', Password='{password}'")
            print(f"Expected: Username='{VALID_USERNAME}', Password='{VALID_PASSWORD}'")


            if username == VALID_USERNAME and password == VALID_PASSWORD:
                try:
                    with open(LOG_FILE_NAME, 'r') as f:
                        log_content = f.read()
                    return jsonify({"log_data": log_content})
                except FileNotFoundError:
                    return jsonify({"error": f"Log file '{LOG_FILE_NAME}' not found."}), 404
                except Exception as e:
                    return jsonify({"error": f"Error reading log file: {str(e)}"}), 500
            else:
                return jsonify({"error": "Invalid credentials"}), 401
        else:
            return jsonify({"error": "Unsupported authentication type"}), 400
    except ValueError:
        return jsonify({"error": "Malformed Authorization header"}), 400
    except Exception as e:
        return jsonify({"error": f"Authentication error: {str(e)}"}), 500


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5138, debug=True)
