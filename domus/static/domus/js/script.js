$(function()
    {
        $('#ON').click
        (
          function ()
          {
            $.post("/domus/update/",{valeur: 1});
          }
        );

        $('#OFF').click
        (
          function ()
          {
            $.post("/domus/update/",{valeur: 0});
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
               }
               else
               {
                 console.log(data);
                 $('#voyant').css('background-color', 'red');
               }
               return false;
             });
          });

        $("#input").change
        (
          function ()
          {
            var imput = $(this).val();
            $.post("/domus/update/",{valeur: imput});
          }
        );
      }
  );




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
