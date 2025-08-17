# Building instructions
***For simplicity, all of this should be done in the same directory as the files in this repo.***
# 1 Install libraries
`pip install Flask flask-cors python-dotenv`


# 2 Set credentials
Make a file called `config.js` and include the following
```js
const VALID_USERNAME = '<YOUR USERNAME HERE>';
const VALID_PASSWORD = '<YOUR PASSWORD HERE>';
```
Make a file called `.env` and include the following
```sh
APP_USERNAME=<YOUR USERNAME HERE>
APP_PASSWORD=<YOUR PASSWORD HERE>
```
***These credentials must match each other***

# 3 Create input and output files
Make the file `ip_list.txt` and `ping_log.txt`.  On each line of `ip_list.txt` enter an IP to be pinged.  Change ping.cpp lines 13 and 14 to point to this file.

# 4 Compile ping.cpp
`g++ ping.cpp -o ping`

# 5 Add the executable to crontab or set a schedule for it to run on a set interval
`crontab -e`

# 6 Run the web server's backend
`python backend_app.py`

