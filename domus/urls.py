from django.conf.urls import url

from . import views

urlpatterns = [
    url(r'^$', views.index,  name='index'),
    url(r'^login/$', views.loginUser, name='loginUser'),
    url(r'^settings/$', views.settings, name='settings'),
    url(r'^control/$', views.controlPage, name='controlPage'),
    url(r'^update/$', views.update, name='update'),
    url(r'^logout/$', views.logoutUser, name='logout'),
]
