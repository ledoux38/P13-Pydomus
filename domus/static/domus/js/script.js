$(function()
{
    update();

    function update()
    {
      $.get("/domus/update/",
      function(data)
       {

         if(data.valeur)
         {
           console.log(data);
           $('#voyant').css('background-color', 'green');
           event.preventDefault();
         }
         else
         {
           console.log(data);
           $('#voyant').css('background-color', 'red');
           event.preventDefault();
         }
       });
    }

    setInterval(update, 5000);

    $('#ON').click
    (
      function ()
      {
        $.post("/domus/update/",{valeur: 1});
        event.preventDefault();
      }
    );

    $('#OFF').click
    (
      function ()
      {
        $.post("/domus/update/",{valeur: 0});
        event.preventDefault();
      }
    );


    $('#voyant').click
    (
      function ()
      {
        $.get("/domus/update/",
        function(data)
         {

           if(data.valeur)
           {
             console.log(data);
             $('#voyant').css('background-color', 'green');
             event.preventDefault();
           }
           else
           {
             console.log(data);
             $('#voyant').css('background-color', 'red');
             event.preventDefault();
           }

         });
      });
});




        // $("#input").change
        // (
        //   function ()
        //   {
        //     var imput = $(this).val();
        //     $.ajax
        //     (
        //       {
        //         url: '/domus/update/',
        //         context: {'valeur': imput},
        //         dataType: 'json',
        //         success: function (context)
        //         {
        //           if (context.valeur)
        //           {
        //             console.log("OK");
        //           }
        //           else
        //           {
        //             console.log("NON");
        //           }
        //         }
        //       }
        //     );
        //   }
        // );
        //
