import os
from bs4 import BeautifulSoup
from playwright.sync_api import sync_playwright, TimeoutError as PlaywrightTimeout
import time


SEASONS = list(range(2016, 2023))
DATA_DIR = "data"
STANDINGS_DIR = os.path.join(DATA_DIR, "standings")
SCORES_DIR = os.path.join(DATA_DIR, "scores")

async def get_html(url, selector, sleep=5, retries=3):
    html = None
    for i in range(1, retries+1): 
        time.sleep(sleep*i)

        try: 
            with sync_playwright() as p: 
                browser = await p.chromium.launch()
                page = await browser.new_page()
                page.goto(url)
                print(await page.title())
                html = await page.inner_html(selector)

        except PlaywrightTimeout:
            print(f"Timeourt error {url}")
            continue

        else:
            break 
    return html

season = 2016
url = f"https://www.basketball-reference.com/leagues/NBA_{season}_games.html"

html = get_html(url, "#content .filter")

print(html)

#soup = BeautifulSoup(html)
#links = soup.find_all("a")
#href = [l["href"] for l in links]

