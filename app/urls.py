from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('home/', views.home, name='home'),
    path('login/', views.login_view, name='login_view'), 
    path('register/', views.register_view, name='register_view'),
    path('create/', views.create_post, name='create_post'),
    path('user/<str:username>', views.user_info, name='user_info'),
]