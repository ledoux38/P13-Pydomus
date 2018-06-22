from django.shortcuts import render, get_object_or_404
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.db import transaction, IntegrityError
from django.http import HttpResponse
# Create your views here.

def index(request):
    message = "Hello world!"
    return HttpResponse(message)
