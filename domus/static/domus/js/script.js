$(function()
    {
        $('#ON').click
        (
          function()
          {
            $.post("127.0.0.1:8000/control/?valeur=1");
          }
        )

        $('#OFF').click
        (
          function()
          {
            $.post("127.0.0.1:8000/control/?valeur=0");
          }
        )

        $("#input").change
        (
          function ()
          {
            var imput = $(this).val();
            $.post("/domus/update/",{valeur: imput});
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
    }
);
