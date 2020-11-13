from cs50 import SQL
from flask import Flask, request, render_template, session, redirect
from flask_session import Session

app = Flask(__name__)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///c50.db")


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/challenge0", methods=["GET", "POST"])
def challenge0():
    if request.method == "GET":
        if check_completion():
            completed = "true"
        else:
            completed = "false"
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        logged_user = rows[0]["username"]
        progress = []
        progress.append(rows[0]["c01"])
        progress.append(rows[0]["c02"])
        progress.append(rows[0]["c03"])
        progress.append(rows[0]["c04"])
        progress.append(rows[0]["c05"])
        return render_template("challenge0.html", user=logged_user, progress=progress, completed=completed)
    else:
        p01 = request.form.get("p01")
        p02 = request.form.get("p02")
        p03 = request.form.get("p03")
        p04 = request.form.get("p04")
        p05 = request.form.get("p05")
        db.execute("UPDATE users SET c01 = :value1, c02 = :value2, c03 = :value3, c04 = :value4, c05 = :value5 WHERE id = :user", value1=p01, value2=p02, value3=p03, value4=p04, value5=p05, user=session["user_id"])
        return redirect("/challenge0")


@app.route("/challenge1", methods=["GET", "POST"])
def challenge1():
    if request.method == "GET":
        if check_completion():
            completed = "true"
        else:
            completed = "false"
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        logged_user = rows[0]["username"]
        set1 = rows[0]["c10"]
        return render_template("challenge1.html", user=logged_user, set1=set1, completed=completed)
    else:
        p01 = request.form.get("p01")
        db.execute("UPDATE users SET c10 = :value1 WHERE id = :user", value1=p01, user=session["user_id"])
        return redirect("/challenge1")


@app.route("/challenge2", methods=["GET", "POST"])
def challenge2():
    if request.method == "GET":
        if check_completion():
            completed = "true"
        else:
            completed = "false"
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        logged_user = rows[0]["username"]
        set1 = rows[0]["c20"]
        return render_template("challenge2.html", user=logged_user, set1=set1, completed=completed)
    else:
        p01 = request.form.get("p01")
        db.execute("UPDATE users SET c20 = :value1 WHERE id = :user", value1=p01, user=session["user_id"])
        return redirect("/challenge2")


@app.route("/challenge3", methods=["GET", "POST"])
def challenge3():
    if request.method == "GET":
        if check_completion():
            completed = "true"
        else:
            completed = "false"
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        logged_user = rows[0]["username"]
        progress = []
        progress.append(rows[0]["c31"])
        progress.append(rows[0]["c32"])
        progress.append(rows[0]["c33"])
        progress.append(rows[0]["c34"])
        progress.append(rows[0]["c35"])
        return render_template("challenge3.html", user=logged_user, progress=progress, completed=completed)
    else:
        p01 = request.form.get("p01")
        p02 = request.form.get("p02")
        p03 = request.form.get("p03")
        p04 = request.form.get("p04")
        p05 = request.form.get("p05")
        db.execute("UPDATE users SET c31 = :value1, c32 = :value2, c33 = :value3, c34 = :value4, c35 = :value5 WHERE id = :user", value1=p01, value2=p02, value3=p03, value4=p04, value5=p05, user=session["user_id"])
        return redirect("/challenge3")


@app.route("/challenge4", methods=["GET", "POST"])
def challenge4():
    if request.method == "GET":
        if check_completion():
            completed = "true"
        else:
            completed = "false"
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        logged_user = rows[0]["username"]
        progress = []
        progress.append(rows[0]["c41"])
        progress.append(rows[0]["c42"])
        progress.append(rows[0]["c43"])
        progress.append(rows[0]["c44"])
        progress.append(rows[0]["c45"])
        return render_template("challenge4.html", user=logged_user, progress=progress, completed=completed)
    else:
        p01 = request.form.get("p01")
        p02 = request.form.get("p02")
        p03 = request.form.get("p03")
        p04 = request.form.get("p04")
        p05 = request.form.get("p05")
        db.execute("UPDATE users SET c41 = :value1, c42 = :value2, c43 = :value3, c44 = :value4, c45 = :value5 WHERE id = :user", value1=p01, value2=p02, value3=p03, value4=p04, value5=p05, user=session["user_id"])
        return redirect("/challenge4")


@app.route("/about")
def about():
    if check_completion():
        completed = "true"
    else:
        completed = "false"
    rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
    logged_user = rows[0]["username"]
    return render_template("about.html", user=logged_user, completed=completed)


@app.route("/join", methods=["GET", "POST"])
def join():
    if request.method == "GET":
        return render_template("join.html")
    else:
        username = request.form.get("username")
        already_used = db.execute("SELECT username FROM users WHERE username = :username", username=username)
        if already_used:
            return redirect("/register_error")
        else:
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=hash_password(request.form.get("password")))
            rows = db.execute("SELECT * FROM users WHERE username = :user", user=username)
            session["user_id"] = rows[0]["id"]
            return redirect("/about")


@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "GET":
        return render_template("login.html")
    else:
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if len(rows) == 1 and rows[0]["hash"] == hash_password(request.form.get("password")):
            session["user_id"] = rows[0]["id"]
            return redirect("/about")
        else:
            return redirect("/login_error")


@app.route("/register_error")
def register_error():
    return render_template("register_error.html")


@app.route("/login_error")
def login_error():
    return render_template("/login_error.html")


@app.route("/logout")
def logout():
    session.clear()
    return redirect("/")


@app.route("/final")
def final():
    rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
    logged_user = rows[0]["username"]
    return render_template("final.html", user=logged_user)


def hash_password(password):
    hash = 0
    for i in range(len(password)):
        hash += ord(password[i]) * 31 * i
    return hash


def check_completion():
    challenges = db.execute("SELECT * FROM users WHERE id = :user", user=session["user_id"])
    for key in challenges[0]:
        if key == "id" or key == "username" or key == "hash":
            continue
        elif key == "c10" or key == "c20":
            if challenges[0][key] != 50:
                return False
        else:
            if challenges[0][key] != 10:
                return False
    return True
