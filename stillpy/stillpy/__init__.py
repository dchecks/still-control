import logging
import os
import time
from datetime import datetime
from logging.handlers import TimedRotatingFileHandler

LOG_FORMAT_STR = "%(asctime)s - %(levelname)-5s - %(name)-20.20s - %(message)s - (%(filename)s:%(lineno)s)"

logging.basicConfig(
    level=logging.DEBUG,
    format=LOG_FORMAT_STR,
    handlers=[logging.StreamHandler()])
logging.getLogger("__name__").setLevel(logging.DEBUG)

LOG_PATH = os.environ.get("LOG_PATH", '/opt/brew/logs')
os.makedirs(LOG_PATH, exist_ok=True)
formatter = logging.Formatter(LOG_FORMAT_STR)
formatter.converter = time.gmtime

date_time_string = datetime.now().strftime("%Y%m%d_%H%M%S")
log_file_name = f"brew_{date_time_string}.log"
handler = TimedRotatingFileHandler(LOG_PATH + log_file_name, when="midnight", backupCount=10, utc=True)
handler.setFormatter(formatter)
logging.getLogger().addHandler(handler)

# Console logger
console_handler = logging.StreamHandler()
console_handler.setFormatter(formatter)

# Add the console handler to the root logger
logging.getLogger().addHandler(console_handler)
