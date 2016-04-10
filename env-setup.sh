#!/bin/bash

echo "[alias]
	hist = log --graph --pretty=format:'%Cred%h%Creset %s -%C(yellow)%d%Creset% Cgreen[%an]%Creset %Cgreen(%cr)%Creset' --abbrev-commit --date=relative" >> .git/config


git config --global color.status auto
git config --global color.add auto
