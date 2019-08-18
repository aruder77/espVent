#!/bin/bash
docker run -d -p 8080:80 -v /Users/axel/Documents/Projects/Arduino/espVent/otaServer/data:/usr/share/nginx/html:ro nginx:stable 
