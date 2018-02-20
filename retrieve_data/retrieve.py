from bs4 import BeautifulSoup
import json
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By

def make_url(year):
    return "https://uselectionatlas.org/RESULTS/data.php?%20ev=1&rnk=1&mar=1&per=1&vot=1&sort=tbl_v.%20margin%3AMargin&fips=0&search=&search_name=&datatype=national&f=1&off=0&year=" + str(year)

def get_candidates(table):
    head = table.find("thead")
    candidates = {}, []
    started = False
    for column in head.find("tr").find_all("td"):
        if column.text == "%Margin":
            started = True
            continue
        if started:
            if column.text == "Other":
                break
            candidates[0][column.find("span")['style']] = str(column.text)
            candidates[1].append(str(column.text))
    return candidates

def get_electoral_votes(table, candidates):
    foot = table.find("tfoot").find_all("tr")[0]
    started = False
    votes = {}
    for column in foot.find_all("td"):
        if column.text == "Total":
            started = True
            continue
        if started and len(votes) < len(candidates):
            try:
                val = int(column.text)
                color = column.find('span')['style']
                votes[candidates[color]] = val
            except ValueError:
                break
            except (KeyError, TypeError):
                continue
    return votes

def get_second_place_index(votes, candidates):
    second_most_votes = sorted(votes.values())[-2]
    for i in range(len(candidates)):
        try:
            if votes[candidates[i]] == second_most_votes:
                return i
        except Exception:
            continue

def get_second_place_loss_margins(table, candidates, votes):
    body = table.find("tbody")
    second_place_index = get_second_place_index(votes, candidates)
    states = {}
    for row in body.find_all("tr"):
        columns = row.find_all("td")
        name = str(columns[2].text)
        value = 0
        for i in range(len(candidates) + 1):
            if ',' in columns[3 + i].text:
                break
            else:
                value += int(columns[3 + i].text)
        started = False
        votes_in_state = []
        for column in columns:
            if "%" in column.text:
                started = True
            if started and "%" not in column.text and len(votes_in_state) < len(candidates):
                votes_in_state.append(int(column.text.replace(",", "")))
        if sum(votes_in_state) > 0 and max(votes_in_state) > votes_in_state[second_place_index]:
            states[name] = (value, (max(votes_in_state) - votes_in_state[second_place_index])/2 + 1)

    return states

def get_total_votes(table):
    foot = table.find("tfoot").find_all("tr")[0]

    for column in foot.find_all("td"):
        if "," in column.text:
            return int(column.text.replace(",", ""))

if __name__ == '__main__':
    data = []
    for year in range(1824, 2017, 4):
        print year
        driver = webdriver.PhantomJS()
        driver.get(make_url(year))
        element_present = EC.presence_of_element_located((By.CSS_SELECTOR, '#datatable tbody tr'))
        for i in range(10):
            try:
                WebDriverWait(driver, 1).until(element_present)
            except TimeoutException:
                print("Time out :(")
        response = driver.page_source
        table = BeautifulSoup(response, "lxml").find("table", {"id":"datatable"})
        candidates = get_candidates(table)
        print candidates
        total = get_total_votes(table)
        print total
        votes = get_electoral_votes(table, candidates[0])
        print votes
        candidates = candidates[1]
        states = get_second_place_loss_margins(table, candidates, votes)
        print states
        print
        data.append({'year' : year, 'candidates' : candidates, 'total' : total, 'votes' : votes, 'states' : states})

    with open('../data/data.json', 'w') as outfile:
        json.dump(data, outfile)
