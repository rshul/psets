from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    #must be implemented fourth
    
    rows = db.execute("SELECT symbol, name, shares FROM portfolio INNER JOIN companies ON portfolio.sym_id = companies.id WHERE user_id = :user_id AND shares > 0",user_id = session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id= :user_id",user_id = session["user_id"] )
    if rows:
        prices = [lookup(row["symbol"])["price"] for row in rows]
        summ = sum([row["shares"] * price for row, price in zip(rows, prices)])
        rows= zip(rows,prices)
        return render_template("index.html", rows=rows, summ = summ, cash=cash[0]["cash"], user = session["username"])
    return render_template("index.html",cash=cash[0]["cash"], user = session["username"])

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # must be imlenented third
    """Buy shares of stock."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        else:
            result = lookup(request.form.get("symbol"))
            if not result:
                return apology("invalid symbol")
                
        if not request.form.get("shares"):
            return apology("must provide", "number of shares")
        elif not request.form.get("shares").isdigit() or int(request.form.get("shares")) <= 0:
            return apology("must be greater than 0 and integer")
            
        shares = int(request.form.get("shares"))
        price = result["price"]
        rows = db.execute("SELECT cash FROM users WHERE id = :uid", uid = session["user_id"])
        if rows[0]["cash"] < shares * price:
            return apology("you can't afford")
        else:
            sym_id = db.execute("SELECT * FROM companies WHERE  symbol= :symbol ", symbol = result["symbol"])
            if not sym_id:
                sym_id = db.execute("INSERT INTO companies (symbol, name) VALUES(:symbol, :name)", 
                                    symbol=result["symbol"], name=result["name"])
            else:
                sym_id = sym_id[0]["id"]
            isshares = db.execute("SELECT shares FROM portfolio WHERE user_id= :user_id AND sym_id= :sym_id",user_id=session["user_id"], sym_id = sym_id)
            if isshares:
                db.execute("UPDATE portfolio SET shares= :shares WHERE user_id= :user_id AND sym_id= :sym_id",shares = isshares[0]["shares"] + shares, user_id = session["user_id"], sym_id = sym_id)
            else:
                db.execute("INSERT INTO portfolio (user_id, sym_id, shares) VALUES (:user_id, :sym_id, :shares)", user_id=session["user_id"], sym_id = sym_id, shares = shares)
                
            db.execute("INSERT INTO transactions (user_id, sym_id, shares, price) VALUES(:user_id, :sym_id, :shares, :price)", 
                        user_id = session["user_id"], sym_id=sym_id, shares=shares, price = price)
            db.execute("UPDATE users SET cash= :cash WHERE id= :user_id", 
                        cash = rows[0]["cash"] - shares * price, user_id = session["user_id"])
            flash("bought")
            return redirect(url_for("index"))
    
    
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    #must be implemented sixth (last)
    """Show history of transactions."""
    rows = db.execute("SELECT * FROM transactions INNER JOIN companies ON transactions.sym_id = companies.id WHERE user_id= :user_id", user_id = session["user_id"])
    
    return render_template("history.html",rows = rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = request.form.get("username")

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    #must be imlplemented second
    if request.method == "POST":
        qt = lookup(request.form.get("symbol"))
        if not qt:
            return apology("invalid symbol")
        else:
            return render_template("quoted.html", company = qt["name"], 
                                symbol = qt["symbol"], price = qt["price"])
        
    
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # must be implemented first
    if request.method == "POST":
        
        if not request.form.get("username"):
            return apology("must provide username")
        rows = db.execute("SELECT * FROM users WHERE username= :username", username = request.form.get("username"))
        if len(rows) == 1:
            return apology("username taken")
            
        if not request.form.get("password"):
            return apology("must provide password")
        
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match")
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :psw)", 
                username = request.form.get("username"), 
                psw = pwd_context.encrypt(request.form.get("password")))
        rows = db.execute("SELECT * FROM users WHERE username= :username", username = request.form.get("username"))
        session["user_id"] = rows[0]["id"]
        session["username"] = request.form.get("username")
        return redirect(url_for("index"))
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        
        if not request.form.get("symbol"):
            return apology("must be symbol")
        else:
            rows = db.execute("SELECT symbol, sym_id, shares FROM portfolio INNER JOIN companies ON portfolio.sym_id = companies.id WHERE user_id= :user_id AND shares > 0", user_id = session["user_id"])
            if not rows:
                return apology("no stocks")
        if not request.form.get("shares") or not request.form.get("shares").isdigit():
            return apology("must be number of shares")
        elif rows[0]["shares"] < int(request.form.get("shares")) or int(request.form.get("shares")) <= 0:
            return apology("wrong number of shares")
        price = lookup(request.form.get("symbol"))
        if not price:
            return apology("wrong symbol")
        price = price["price"]
        db.execute("INSERT INTO transactions (user_id, sym_id, shares, price) VALUES (:user_id, :sym_id, :shares, :nprice)", user_id = session["user_id"], sym_id = rows[0]["sym_id"], shares = -int(request.form.get("shares")), nprice = price)
        db.execute("UPDATE users SET cash = cash + :income WHERE id= :user_id", income = int(request.form.get("shares")) * price, user_id = session["user_id"] )
        db.execute("UPDATE portfolio SET shares = shares - :sold WHERE user_id= :user_id AND sym_id= :sym_id", sold = int(request.form.get("shares")), user_id = session["user_id"], sym_id = request.form.get("symbol"))

        return redirect(url_for("index"))
        
        
    else:
        return render_template("sell.html")
@app.route("/password", methods=["GET", "POST"])
def password():
    if request.method == "POST":
        if not request.form.get("old"):
            return apology("no password")
        elif not request.form.get("new"):
            return apology("no new password")
        elif not request.form.get("confirmation"):
            return apology("no confirmation")
        rows = db.execute("SELECT hash FROM users WHERE id= :user_id", user_id = session["user_id"])
        if len(rows) != 1 or not pwd_context.verify(request.form.get("old"), rows[0]["hash"]):
            return apology("invalid password")
        if request.form.get("new") != request.form.get("confirmation"):
            return apology("password don't match")
        psw = pwd_context.encrypt(request.form.get("new"))
        db.execute("UPDATE users SET hash= :psw WHERE id= :user_id", psw = psw, user_id = session["user_id"])
        flash("password changed")
        return redirect(url_for("index"))
        
    else:
        return render_template("password.html")