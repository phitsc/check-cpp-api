#!/usr/bin/env bash

read -r project_name < project_name.cfg

python3 api-utils-common/scripts/make.py --tool "${project_name}" $@

