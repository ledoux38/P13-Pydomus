from django.contrib.auth.decorators import login_required
from django.contrib.auth import get_user_model
from django.shortcuts import render, get_object_or_404
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.db import transaction, IntegrityError
from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout

from .forms import Login

# Create your views here.

@login_required(login_url='/domus/login')
def index(request):
    """ensures the display of the home page"""
    return render(request, 'domus/index.html')

def login(request):
    """ensures the display of the login page
    as well as the authentication
    procedure of the user
    """
    # If the method is of type POST
    if request.method == 'POST':
        form = Login(request.POST)
        if form.is_valid():
                    # Processing form data
                    name = form.cleaned_data['name']
                    password = form.cleaned_data['password']
                    user = authenticate(username=name, password=password)
                    #We check if the data is correct
                    if user:
                        login(request, user)
                        return render(request, 'domus/index.html')
    # If the method is of type GET
    else:
        form = Login()
        context = {"login": form}
        return render(request, 'domus/login.html', context)

def settings(request):
    """ensures the display of the login page
    as well as the authentication
    procedure of the user
    """
    message = "You asked for the settings page"
    return HttpResponse(message)
