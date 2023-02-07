# -*- coding: utf-8 -*-
"""
Created on Thu Feb  2 19:34:08 2023

@author: Michael
"""

from pybaseball import batting_stats

START = 2002
END = 2022

batting = batting_stats(START, END, qual=200)

batting.to_csv("batting.csv")

