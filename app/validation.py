import re

def validateName(name):
    if len(name) > 20:
        return "Provided name is too long"
    else:
        return 0

def validateUsername(username):
    pattern = r'^[A-Za-z0-9]{3,20}$' # 3-20 characters long with non-special characters
    if(re.match(pattern, username)):
        return 0
    else:
        return "Username has to be 3-20 characters long with no special characters"

def validatePassword(password):
    if(len(password) < 6):
        return "Password is too short (has to be at least 6 characters long)"
    else:
        return 0

def validateEmail(email):
    # the pattern below matches email adresses specified by the structure outlined by RFC 5322 specification 
    pattern = r'(?:[a-z0-9!#$%&\'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&\'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])'
    if(re.match(pattern, email)):
        return 0
    else:
        return "Invalid email adress"

# go through each field and validate it
def validate(username, password, email, first_name, last_name):
    v_first_name = validateName(first_name)

    if(v_first_name != 0):
        return (v_first_name)
    
    v_last_name = validateName(last_name)

    if(v_last_name != 0):
        return (v_last_name)
    
    v_email = validateEmail(email)

    if(v_email != 0):
        return (v_email)
    
    v_username = validateUsername(username)

    if(v_username != 0):
        return (v_username)
    
    v_password = validatePassword(password)

    if(v_password != 0):
        return (v_password)
    
    return 0