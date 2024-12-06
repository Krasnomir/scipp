from django.shortcuts import redirect
from django.urls import reverse

# if user is not logged in and tries to access any view other than /login/ or /register/ they will be redirected
class RequireLoginMiddleware:
    def __init__(self, get_response):
        self.get_response = get_response

    def __call__(self, request):
        if not request.user.is_authenticated and not (request.path == reverse('login_view') or request.path == reverse('register_view')):
            return redirect('login_view')
        
        response = self.get_response(request)
        return response
