# Building instructions

# 1 Install libraries
`pip install Flask flask-cors python-dotenv`

# 2 Compile ping.cpp
`g++ ping.cpp -o ping`

### *Add the executable to crontab or set a schedule for it to run on a set interval*

# 3 Set credentials
Make a file called `config.js` and include the following
```js
const VALID_USERNAME = '<YOUR USERNAME HERE>';
const VALID_PASSWORD = '<YOUR PASSWORD HERE>';
```
Make a file called `.env` and include the following
```sh
APP_USERNAME=admin
APP_PASSWORD=pass
```

# 4 Run the web server's backend
`python backend_app.py`

