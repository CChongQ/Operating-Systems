jQuery(function($) {
//BEGIN JQUERY

//_header_shelf.js
/* NAV SHELF - Rethink *************/

    // Minimize the header height on scroll, show surveys
    // Slide control

        var lastScrollTop = 0;

        $(window).scroll(function() {
            var windowWidth = $(window).width();
            var siteLogo = $(".header .site-logo-large");
            var siteHeader = $(".header");
            var searchDrawer = $(".search-drawer-wrapper");

            var scrollTop = $(document).scrollTop();
            var scrollBottom = $(window).scrollTop() + $(window).height() > $(document).height() - 100;
            var st = $(this).scrollTop();

            // shrink the logo - legacy effect
            if (scrollTop > 500 && windowWidth >= 1068) {
                siteLogo.css("width", "75%");
                //siteHeader.css({"top": "-190px", "height": "140px"});
                if (st > lastScrollTop) {
                    siteHeader.css({"top": "-190px", "height": "140px"});
                    searchDrawer.hide(500, function() {
                        searchDrawer.slideUp();
                    });
                } else {
                    siteHeader.css({"top": "0", "height": "140px"});
                }
            } else if (windowWidth >= 1068) {
                siteLogo.css("width", "100%");
                siteHeader.css({"top": "0", "height": "140px"});
            } else if (scrollTop > 500 && windowWidth <= 1068) {
                siteHeader.css({"height": "90px", "top": "-90px", "opacity": "0"});
                searchDrawer.hide(500, function() {
                        searchDrawer.slideUp();
                });
                if (st > lastScrollTop) {
                    siteHeader.css({"height": "90px", "top": "-90px", "opacity": "0"});
                } else {
                    siteHeader.css({"height": "90px", "top": "0", "opacity": "1"})
                }
            }

            lastScrollTop = st;

            // once at bottom
            if (scrollBottom && windowWidth >= 1068) {
                siteHeader.css({"top": "0", "height": "140px"});
            }
            if (scrollBottom && windowWidth <= 1068) {
                siteHeader.css({"top": "0", "height": "90px"});
            }

        });

//_pages.js
/*===Check to see if divs are empty - Audience/Info pages=== TODO, tighten, make more efficient, create plugin*/
  function isLinksEmpty(el) {
    return !$.trim(el.html());
  }
  if (isLinksEmpty($('.row-neutral-uoft-blue.linksBlock.band .container .col-md-12'))) {
    $('.row-neutral-uoft-blue.linksBlock.band').hide();
  } else if (isLinksEmpty($('.infoPage .row-neutral-white.linksBlock .container .col-md-12'))) {
    $('.row-neutral-white.linksBlock').hide();
  }

  function isLinksBlockBlueEmpty(el) {
    return !$.trim(el.html());
  }
  if (isLinksBlockBlueEmpty($('.row-neutral-uoft-blue.linksBlock .container .col-md-12'))) {
    $('.row-neutral-uoft-blue.linksBlock').hide();
  }

  function isbandBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (isbandBlockEmpty($('.band.cont .featureOnlyBlock .col-md-12'))) {
    $('.band.cont').removeClass('band');
  }

  function isbandquoteBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (isbandquoteBlockEmpty($('.band.cont .quoteOnlyBlock .col-md-12'))) {
    $('.band.cont').removeClass('band');
  }

  function iswhiteNewsBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (iswhiteNewsBlockEmpty($('.row-neutral-white.newsBlock .container .panel-pane'))) {
    $('.row-neutral-white.newsBlock').hide();
  }

  function iswhiteLinksBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (iswhiteLinksBlockEmpty($('.row-neutral-white.linksBlock .container .panel-pane'))) {
    $('.row-neutral-white.linksBlock').hide();
  }

  function isfeatureQuoteBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (isfeatureQuoteBlockEmpty($('.featureQuoteBlock .panel-pane'))) {
    $('.featureQuoteBlock').hide();
  }

  function isfooterLinksBlock(el) {
    return !$.trim(el.html());
  }
  if (iscampusBlockEmpty($('.footerLinksBlock .col-md-12 .panel-pane'))) {
    $('.footerLinksBlock').hide();
  }

  function isnewsBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (isnewsBlockEmpty($('.newsBlock.band .container .panel-pane'))) {
    $('.newsBlock.band').hide();
  }

  function isneutralAlumnBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (isneutralAlumnBlockEmpty($('.row-neutral-feat-alum.alumBlock .container .panel-pane'))) {
    $('.row-neutral-feat-alum.alumBlock').hide();
  }

  function isfeatureOnlyBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (isquoteOnlyBlockEmpty($('.featureOnlyBlock div'))) {
    $('.featureOnlyBlock').hide();
    //$('.featureOnlyBlock').parent().hide();
  }

  function isquoteOnlyBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (isquoteOnlyBlockEmpty($('.quoteOnlyBlock div'))) {
    $('.quoteOnlyBlock').hide();
  }

  function istwoColBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (istwoColBlockEmpty($('.twoColBlock .container div'))) {
    $('.twoColBlock').hide();
  }

  function iscampusBlockEmpty(el) {
    return !$.trim(el.html());
  }
  if (iscampusBlockEmpty($('.campusBlock .container div'))) {
    $('.campusBlock').hide();
  }

  function isPageBannerEmpty(el) {
    return !$.trim(el.html());
  }
  if (isPageBannerEmpty($('.pageBannerBlock div'))) {
    $('.pageBannerBlock').removeClass('pageBannerBlock');
  }
  //END Check to see if divs are empty - Audience/Info pages

//_news.js
$(".page-taxonomy-term .media-url").siblings(".title").find('a').contents().unwrap().wrap('<strong/>');
$(".page-news-latest-news .latest-news .views-exposed-widgets .views-submit-button .form-submit").attr("value", "Search");
$(".page-news-find-a-story .latest-news .views-exposed-widgets .views-submit-button .form-submit").after("<span class='resetBtn'><a href='/news/find-a-story?keys=&field_degrees_value=1'>Reset</a></span>");

$(".news-home-banner div.picture").nextAll().wrapAll($("<div></div>").addClass("title-header"));

var emptyOneCol = $('.audiencePage .one-col-body .row .col-xs-12');
var emptyMoreStories = $('.audiencePage .more-stories-body');
var emptyFullWidthStories = $('.audiencePage .full-width.dark');

function isEmptyColBody(el) {
  return !$.trim(el.html());
}
if (isEmptyColBody(emptyOneCol)) {
  // console.log("Empty");
  emptyOneCol.addClass("removeMe").parent().addClass("removeMe").parent().remove();
  emptyMoreStories.addClass("removeMe").remove();
  emptyFullWidthStories.addClass("removeMe").remove();
}

//NEWS TOPICS TAGS
var str = $('.pane-node-field-topic .links li a').text();
$(".links li a").addClass(str);
$('.mailchimp-newsletter-mergefields input[name="mergevars[EMAIL]"]').attr('placeholder', 'Email address');
$(".page-taxonomy-term .messages.warning").insertBefore("form.mailchimp-signup-subscribe-form");
$(".page-taxonomy-term .messages.status").insertBefore("form.mailchimp-signup-subscribe-form");
$(".page-news-contactus .messages.error").insertAfter(".page-news-contactus .contact-form");

$(".sticky-stories-tag a").insertBefore(".news-tags a:nth-of-type(1)");
/*News Home Page Styling for content type*/

$(".news-home .media-icon").parent().css({ 'background-color': '#495667', 'padding': '10px', 'border-radius': '3px' });

$(".news-home .media-icon").siblings('.date').css({ 'position': 'absolute', 'top': '0', 'right': '30px', 'color': '#fff', 'text-align': 'right' });
$(".news-home .media-icon").siblings('.title').css({ 'padding': '0 0 10px 0', 'margin': '0' });
$(".news-home .media-icon").siblings('.title').find("a").css({ 'color': 'white' });
$(".news-home .media-icon").siblings('.picture').css({ 'display': 'none' });
$(".news-home .media-icon, .news-home .youtube,.news-home .flickr-set").siblings('.topic').css({ 'display': 'none' });
$(".news-search .view-content p:contains('embed_content nid=')").remove();
$(".media-body").each(function() {
  var link = $("a:first-child", this).attr("href");
  $(this).siblings(".title").find("a").attr("href", link);
});
$(".contact-form label").each(function() {
  var place = $(this).text()
  $(this).siblings("input").attr('placeholder', place);
});
$(".contact-form textarea").attr("placeholder", "Story Proposal")
$(".contact-form #edit-submit").val("submit");

//For the Tag Template
$(".page-taxonomy.page-taxonomy-term .ds-1col.node.node-story.node-teaser").addClass("container");

/* News Individual Post*/
//var post = $(".related .col3-equal.related-stories");
//if(post){$(".related .pane-title").text("Related Stories");}
if ($(".info .author").text().length <= 3) {
  $(".info .author").text(" ");
}
function isSectionempty(el) {
  return !$.trim(el.html());
}
$(window).ready(function() {
  //if(window.location.href.indexOf("/news/") > -1) { USED BEFORE ON NEWS URL CHANGED BECAUSE THERE ARE DIFFERENT STORY TYPES NOW
  if ($("body").hasClass("story-page")) {

    var checkofficialstatment = $(".feature-picture .official-statement");
    var checkofficialstatmentSmall = $(".small-feature-picture .official-statement");
    if(!isSectionempty(checkofficialstatment) || ( isSectionempty(checkofficialstatment) && isSectionempty(checkofficialstatmentSmall)) ){
      $(".small-feature-picture .panel-pane.pane-custom.pane-4.clearfix").remove();
    }
    var original = $(".original-image").attr("src");
    if (original.length < 15) {
      $(".small-feature-picture .panel-pane.pane-custom.pane-5.clearfix").remove();
    }
  }
})

// Append new authors field to the same line as the authors field
var newAuthors = $(".info .pane-node-field-new-authors-reporters .pane-content").html();
$(".info .authors-internal .pane-content").append(newAuthors);
var newSauthors = $(".info .pane-node-field-new-secondary-author-repor .pane-content").html();
$(".info .authors-external .pane-content").append(newSauthors);

//Append New Tags field into the Tags Field in individuals News Story
var newTags = $(".Tags .pane-node-field-new-story-tags .pane-content").html();
$(".Tags .news-tags .pane-content").append(newTags);

$(".topic-wrapper h3.topic-title").each(function() {
  $(this).siblings(".term-main.last").insertBefore(this);
});
$("body.node-type-story strong").siblings("img.media-image").insertBefore("body.node-type-story .node.node-story.node-teaser h2 a");
$("body.node-type-story img.media-image").closest("body.node-type-story div.node.node-story.node-teaser").addClass("width50");

if ($.trim($(".authors-reporters .image").html()) == '') {
  $(".authors-reporters .image").css({ "display": "none" });
  $(".authors-reporters #page-title").css({ "visibility": "hidden" });
  $(".authors-reporters .email").css({ "display": "none" });
}

//_cities.js
//City Building Blog - match height of news-home block
  $(window).ready(function() {
    if ($("body[class*='page-about-u-of-t-city-building']").length) {
      //console.log("I'm on City Building");
      var maxHeightBlog = -1;
      var maxHeightTrio = -1;
      var uoftBlogTrio = $(".page-about-u-of-t-city-building .linksBlock .pane-block");
      var uoftBlogBlocks = $(".page-about-u-of-t-city-building .pane-latest-news .news-home");

      function resizeTrio() {
        uoftBlogTrio.each(function() {
          maxHeightTrio = maxHeightTrio > $(this).height() ? maxHeightTrio : $(this).height();
        });

        uoftBlogTrio.each(function() {
          $(this).height(maxHeightTrio);
        });
      }

      resizeTrio();

      //resize news block for blogs
      function resizeNewsBlog() {

        uoftBlogBlocks.each(function() {
          maxHeightBlog = maxHeightBlog > $(this).height() ? maxHeightBlog : $(this).height();
        });

        uoftBlogBlocks.each(function() {
          $(this).height(maxHeightBlog);
        });
      }

      resizeNewsBlog();

      //resize the window, if tablet or less, height is auto
      $(window).resize(function() {
        if ($(window).width() > 992) {
          resizeNewsBlog();
          resizeTrio();

        } else {
          uoftBlogBlocks.css('height', 'auto');
          uoftBlogTrio.css('height', 'auto');
        }
      });

    }
  })
//_bulletin.js
//The Bulletin
// Control Feature Picture & Small Image
if ( $(".node-type-the-bulletin .feature-picture img").length !== 0){
    $(".node-type-the-bulletin .small-picture").css("display","none");
}

if ($(".node-type-the-bulletin .small-picture img").length !== 0){
  $(".node-type-the-bulletin .feature-picture,.node-type-the-bulletin .feature-picture h1").css("display", "none");
}   


  $('#calendar').fullCalendar({
    customButtons: {
      myCustomButton: { text: 'Year' }
    },
    header: {
      left: 'prev title next',
      center: "",
      right: 'prevYear myCustomButton nextYear'
    },
    eventTextColor: "#fff",
    events: '//sites.utoronto.ca/ebulletin/bulletinarchive.php?callback=?',
    eventRender: function(event, element) {
      var dataToFind = moment(event.start).format('YYYY-MM-DD');
      //make the title a link
      var linkformat = dataToFind.substr(2, 8);
      if (element.hasClass("pdf")) {
        $("td[data-date='" + dataToFind + "']").addClass('bulletinDay').find("span").wrap('<a href="http://sites.utoronto.ca/ebulletin/_bulletin/' + dataToFind + '.pdf"></a>');
      } else {
        $("td[data-date='" + dataToFind + "']").addClass('bulletinDay').find("span").wrap('<a href="http://sites.utoronto.ca/ebulletin/' + linkformat + '/bulletin-' + linkformat + '.html"></a>');
      }
    }
  });
  $('.pop-over').popover({
    trigger: 'focus'
  })
  $('.popup').click(function() {
    $(this).next('div.nextpopup').modal({ show: true, });
  });
  $('.close').click(function() {
    $(this).parent('.form-popup').modal({ show: false, });
  });

  // ------ make all cards same height for Bulletin News
  equalheight = function(container) {

    var currentTallest = 0,
      currentRowStart = 0,
      rowDivs = new Array(),
      $el,
      topPosition = 0;
    $(container).each(function() {
      $el = $(this);
      $($el).height('auto')
      topPostion = $el.position().top;

      if (currentRowStart != topPostion) {
        for (currentDiv = 0; currentDiv < rowDivs.length; currentDiv++) {
          rowDivs[currentDiv].height(currentTallest);
        }
        rowDivs.length = 0; // empty the array
        currentRowStart = topPostion;
        currentTallest = $el.height();
        rowDivs.push($el);
      } else {
        rowDivs.push($el);
        currentTallest = (currentTallest < $el.height()) ? ($el.height()) : (currentTallest);
      }
      for (currentDiv = 0; currentDiv < rowDivs.length; currentDiv++) {
        rowDivs[currentDiv].height(currentTallest);
      }
    });
  }

  // ------ Use match media to fire these functions only above a certain breakpoint
  if (matchMedia('only screen and (min-width: 768px)').matches) {
    $(window).on('load', function() {
      equalheight('.bulletin-news .subhead');
    });

    $(window).resize(function() {
      equalheight('.bulletin-news .subhead');
    });
  }


function accord(id) {
    var $accord = $(id);
    var $allLinks = $(".toggle");
    $accord.hide();
    $allLinks.click(function(e) {
      e.preventDefault();
      $allLinks.removeClass('close-icon');
      var $this = $(this).parent().next();
      var $linkEl = $(this);
      $accord.slideUp('normal');
      if ($this.is(':hidden') == true) {
        $linkEl.addClass('close-icon');
        $this.slideDown('normal');
      }
    });

  }
  // ------ Accordion functionality for Bulletin page
  if (window.location.href.indexOf("bulletin") > -1) {
    accord(".accord-section");
  }
//_ironchef.js
//CountDown Timer
  if (window.location.href.indexOf("ironchef") > -1) {
    //countdown("Feb 25, 2017 08:00:00", "ic-countdown");
  }

  function countdown($date, $id) {
    // Set the date we're counting down to
    //var countDownDate = new Date("Jan 5, 2018 15:37:25").getTime();
    var countDownDate = new Date($date).getTime();

    // Update the count down every 1 second
    var x = setInterval(function() {

      // Get todays date and time
      var now = new Date().getTime();

      // Find the distance between now an the count down date
      var distance = countDownDate - now;

      // Time calculations for days, hours, minutes and seconds
      var days = Math.floor(distance / (1000 * 60 * 60 * 24));
      var hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
      var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
      var seconds = Math.floor((distance % (1000 * 60)) / 1000);

      // Display the result in the element with id="demo"
      document.getElementById($id).innerHTML = days + "d " + hours + "h " +
        minutes + "m " + seconds + "s ";

      // If the count down is finished, write some text 
      if (distance < 0) {
        clearInterval(x);
        document.getElementById($id).innerHTML = "EXPIRED";
      }
    }, 1000);

  }
  $(".ironchef-body .desktopyoutube").click(function(e) {
    var utube = $(this).siblings(".youtube");
    var closebnt = $(this).siblings(".btnc").find(".closebtn");
    utube.addClass("showvideo");
    utube.removeClass("hidevideo");
    closebnt.addClass("showvideo");
    closebnt.removeClass("hidevideo");
  });
  $(".ironchef-body .fa.fa-play").click(function(e) {
    var utube = $(this).parent().siblings(".youtube");
    var closebnt = $(this).parent().siblings(".btnc").find(".closebtn");

    utube.addClass("showvideo");
    utube.removeClass("hidevideo");
    closebnt.addClass("showvideo");
    closebnt.removeClass("hidevideo");
  });
  $(".ironchef-body .btnc").click(function(e) {
    var utube = $(this).siblings(".youtube");
    utube.addClass("hidevideo");
    utube.removeClass("showvideo");
    $(".ironchef-body .video .closebtn").addClass("hidevideo");
    $(".ironchef-body .video .closebtn").removeClass("showvideo");
  });
  $(".ironchef-body .bio .closebtn").click(function(e) {
    var bio = $(this).parents(".bio");
    bio.addClass("hidevideo");
    bio.removeClass("showvideo");
    $('html').css('overflow', 'scroll');
    $('body').unbind('touchmove');
  });
  $(".ironchef-body .team .title a").click(function(e) {
    e.preventDefault();
    var openbio = $(this).parent().siblings(".bio");
    openbio.addClass("showvideo");
    openbio.removeClass("hidevideo");
    $('html').css('overflow', 'hidden');
    $('body').bind('touchmove', function(e) {
      e.preventDefault()
    });
  });

  //Resizer Mobile JS 
  window.sizeCheck = Number(0);
  $(window).on("resize", function() {
    $(window).off('resize', resizeMaster);
    var mobWidth = Number(780);
    var deskWidth = Number(1024);
    if ($(window).width() < mobWidth && window.sizeCheck != 3) {
      resizeMaster(3);
      window.sizeCheck = 3;
    } else if ($(window).width() > mobWidth && $(window).width() < deskWidth && window.sizeCheck != 2) {
      resizeMaster(2);
      window.sizeCheck = 2;
    } else if ($(window).width() > deskWidth && window.sizeCheck != 1) {
      resizeMaster(1);
      window.sizeCheck = 1;
    }
  }).resize(); //resizeMaster size checker
  function resizeMaster(res) {

    // Tablet = 2 , Mobile = 3, Desktop = 1 
    if (res >= 2) {
      $("#news-menu-wrapper").insertAfter(".news-banner-title");
      $(".page-news-back-to-school #news-menu-wrapper").insertBefore(".boilerplateBlock .row");
      $(".news-home .media-icon").siblings('.date').css({ 'background': 'transparent', 'margin-top': '0' });
      $(".news-home .media-icon").siblings('.title').css({ 'background': 'transparent' });
      $('.mobile-news-menu .fa-caret-down').click(function() {
        $('#block-menu-block-5').show();
        $('.mobile-news-menu .fa.fa-caret-down').hide();
        $('.mobile-news-menu .fa.fa-times').show();
      });
      $('.mobile-news-menu  .fa-times').click(function() {
        $('#block-menu-block-5').hide();
        $('.mobile-news-menu  .fa.fa-times').hide();
        $('.mobile-news-menu .fa.fa-caret-down').show();

        // remove the class to hide the scroll bars on html - hideDoubleScroll found in _newHeader
      });
      $(".ironchef-body team>div, .ironchef-body video >div, ironchef-body news >div").hide();
      $(".page-news-find-a-story .container-inline-date").addClass("container popup-date").removeClass("container-inline-date");
      $(".ironchef-wrapper .col-md-4").removeClass("equalheight").css('height', '');
      $(".ironchef-body .team>div,.ironchef-body .video>div,.ironchef-body .news>div ").addClass("ironchef-section").hide();
      if (window.location.href.indexOf("ironchef") > -1) {
        accord(".ironchef-section");
      }

    } else {
      $(".banner-wrapper .col-md-4").addClass("equalheight");
      equalheight('.equalheight');
      $(".ironchef-body .team>div,.ironchef-body .video>div,.ironchef-body .news>div ").removeClass("ironchef-section").show();
      $(".page-news-back-to-school .bts .news-home .media-icon, .page-news .col3-equal .news-home.mobile-news-home .media-icon").parent().css({ 'border-right': '20px solid #fff' });
      $(".page-news .col3-equal.more-stories-row3:not(.bts) .news-home.mobile-news-home .media-icon, .page-news .col2-leftbig:not(.bts) .news-home.mobile-news-home .media-icon, .page-news .col2-equal:not(.bts) .news-home.mobile-news-home .media-icon").parent().css({ 'border-right': '20px solid  #EDF2F8' });
      $("#news-menu-wrapper").insertAfter(".sub-menu-wrapper");
      $(".small-feature-picture .special").insertBefore(".info");
      $(".feature-picture .special").insertBefore(".info");
      $(".page-news-find-a-story popup-date").addClass("container-inline-date").removeClass("container");
    }
  }
//_scaffold.js
  // TOOLTIP activation
  $('.photo-caption-toggle').tooltip();

  //match height of white blocks in links row 
  var maxHeight = -1;
  var linksWhiteBg = $('.linksBlock .white-bg');

  linksWhiteBg.each(function() {
    maxHeight = maxHeight > $(this).height() ? maxHeight : $(this).height();
  });

  linksWhiteBg.each(function() {
    $(this).height(maxHeight);
  });

  //For AODA
  //remove empty divs
  $(".homeEventsBlock .node h2").remove();
  $(".homeEventsBlock h2.pane-title").remove();
  $(".page-alumni .pane-pull-quotes h2.pane-title").remove();
  $(".page-academics .featureOnlyBlock .changeable .--title").remove();
  $(".page-university-life-campuses .pane-content .node h2").remove();
  //alt tags
  var cfText = $(".view-changeable-feature h2.field-content").text();
  $(".view-id-changeable_feature .field-content img").attr("alt", cfText);
  var cfText1 = $(".featureQuoteBlock .col-md-4 .pane-title").text();
  $(".col-md-8 .view-id-changeable_feature img").attr("alt", cfText1);
  var cfText2 = $(".twoColBlock .col-md-4 .pane-title").text();
  $(".col-md-8 .view-id-changeable_feature img").attr("alt", cfText2);
  var pullQText = $(".view-pull-quotes .name").text();
  $(".view-pull-quotes .pq--round img").attr("alt", pullQText);
  var bannerText = $(".page-banner-title a").text();
  $(".views-field-field-banner img").attr("alt", bannerText);
  var newsText = $(".view-latest-news h4 a").text();
  $(".view-latest-news .field-content img").attr("alt", "news item image");
  //add labels to forms
  $("#edit-jump").attr("aria-label", "label");
  $("#edit-field-campus-value").attr("aria-label", "label");
  $("#edit-submit-social-media-directory-tabs").attr("aria-label", "label");

  //CAROUSEL in View 
  $(".carouselBlock .donor-container:first").addClass("active");
  $(".carouselBlock .donor-container").addClass("item");
  $(".carouselBlock .view-content .donor-container").appendTo(".carouselBlock #carousel-example-generic .carousel-inner");


  $(".story-content .pane-node-body p strong:contains('[embed_content')").hide();
  $(".story-content .pane-node-body p strong:contains('(class=')").hide();

  /* Add clearfix - non-essential so not a big deal if js doesn't load */
  $('.newsBlock .view-content').addClass('clearfix');
  $('.linksBlock .pane-content').addClass('clearfix');
  $('.footerLinksBlock .pane-content').addClass('clearfix');
  $('.panel-pane').addClass('clearfix');
  $('.campusBlock .pane-content').addClass('clearfix');
  /* Add the BS markup to pullquotes created from Views */
  $('.pane-pull-quotes .panel-col-first').addClass('col-md-6');
  $('.pane-pull-quotes .panel-col-last').addClass('col-md-6');
  /* Add container to subnav menus to center */
  $('.academics-sub-nav .menu-block-wrapper').addClass('container');
  $('.universitylife-sub-nav .menu-block-wrapper').addClass('container');
  $('.aboutUoft-sub-nav .menu-block-wrapper').addClass('container');

  
  //_temp.js
  //YT Click Override - REMOVE THIS WHEN BTS DONE
  // Handle resize event 
  $(window).ready(ytOverride);
  $(window).on('resize', ytOverride);

  function ytOverride() {
    if ($(window).width() >= 768) {
      $(".vid-cont .caption_wrapper").click(function() {
        //console.log("CLICK YT");
        window.location.href = '/back-to-school';
      });
    }
  }


//_bts.js
//SMOOTH SCROLL PLUGIN - CSS-TRICKS

  $('.page-back-to-school a[href*="#"], .entrepreneurs a[href*="#"]')
    // Remove links that don't actually link to anything
    .not('[href="#"]')
    .not('[href="#0"]')
    .click(function(event) {
      // On-page links
      if (
        location.pathname.replace(/^\//, '') == this.pathname.replace(/^\//, '') &&
        location.hostname == this.hostname
      ) {
        // Figure out element to scroll to
        var target = $(this.hash);
        target = target.length ? target : $('[name=' + this.hash.slice(1) + ']');
        // Does a scroll target exist?
        if (target.length) {
          // Only prevent default if animation is actually gonna happen
          event.preventDefault();
          $('html, body').animate({
            scrollTop: target.offset().top - 145
          }, 1000, function() {
            // Callback after animation
            // Must change focus!
            var $target = $(target);
            $target.focus();
            if ($target.is(":focus")) { // Checking if the target was focused
              return false;
            } else {
              $target.attr('tabindex', '-1'); // Adding tabindex for elements not focusable
              $target.focus(); // Set focus again
            };
          });
        }
      }
    });


//TODO make this load faster
  function btsMenuTakeover() {
    //console.log("Takeover");
    $('.subMenuCont .menu a[href*="visitors"]').each(function() {
      $('.page-home .subMenuCont .menu, .page-future-students .subMenuCont .menu, .page-current-students .subMenuCont .menu, .page-academics-programs-directory .header .sub-menu-wrapper .subMenuCont .menu-block-3 .menu, .page-back-to-school .subMenuCont .menu, .page-news .subMenuCont .menu').addClass('forceHov');
      $('.page-home .subMenuCont .menu .last.leaf, .page-future-students .subMenuCont .menu .last.leaf, .page-current-students .subMenuCont .menu .last.leaf, .page-academics-programs-directory .header .sub-menu-wrapper .subMenuCont .menu-block-3 .menu .last, .page-back-to-school .subMenuCont .menu .last.leaf, .page-news .subMenuCont .menu .last.leaf').addClass('forceHov');
    });
  }

  $("#header").ready(btsMenuTakeover);

  //BTS - TODO: move to separate bts.js file
  var btsPage = window.location.pathname == '/back-to-school';

  if (btsPage) {
    $(window).ready(btsWrap);
    $(window).on('resize', btsBoxHeight);

    $("#mailchimp-signup-subscribe-block-the-bulletin-subscribe-form-form").addClass("clearfix");

    function btsWrap() {
      // console.log("Initial Load");
      $(".utm-bts-events, .utsg-bts-events, .utsc-bts-events").wrapAll("<div class='col-md-12 eventsWrap'></div>");
      $(".bts-news-signup").wrapAll("<div class='bts-signupWrap clearfix'><div class='container'></div></div>");
      $(".linksBlock").wrapAll("<a href='/news'></a>");
      btsBoxHeight();

    }

    function btsBoxHeight() {
      var btsBox = $('.eventsWrap .col-md-4 .pane-content');
      btsBox.each(function() {
        $(this).css('height', 'auto');
      });

      btsRecalcBoxHeight();
    }

    function btsRecalcBoxHeight() {
      // console.log("Recalculate height");
      var maxHeight = -1;
      var btsBox = $('.eventsWrap .col-md-4 .pane-content');

      //assigns new height
      btsBox.each(function() {
        maxHeight = maxHeight > $(this).height() ? maxHeight : $(this).height();
      });

      btsBox.each(function() {
        $(this).height(maxHeight);
      });
    }
  }

//_video.js
//PLYR
  var homeRoot = window.location.pathname == '/';
  var home = window.location.pathname == '/home';
  if (homeRoot || home) {
    // VIDEO WIDGET - plyr.js
    // Setup the player
    var instances = plyr.setup({
      debug: false,
      title: 'Video',
      tooltips: {
        controls: true
      },
      captions: {
        defaultActive: false
      }
    });
  }
  // Handle resize event 
  $(window).ready(loadResizeVid);
  $(window).on('resize', loadResizeVid);

  function loadResizeVid() {
    $('.duration').fadeIn('fast');
    $('.pane-video-widget .cat-desc p:first-of-type').append($('.duration'));

    if ($(window).width() <= 768) {
      $('.pane-video-widget .yt').insertBefore('.pane-video-widget .content');
    } else {
      $('.pane-video-widget .content').insertBefore('.pane-video-widget .yt');
    }

  }
//_twitter.js
//TWEET THIS
  if ($(".quote").is(':visible')) {
    //console.log("visible quote");
    $(".quote").append("<button id='tweetThis' class='btn btn-primary'><i class='fa fa-twitter'></i> Tweet this</button>");
  }

  //Tweet this Block Quote
  $("#tweetThis").click(function() {
    var phrase = $('.quote').clone();
    $('#tweetThis', phrase).remove();
    var newPhrase = phrase.text();
    if (newPhrase.length > 116) {
      newPhrase = newPhrase.substr(0, 113) + "...";
    }

    var tweetUrl = 'https://twitter.com/share?text=' +
      encodeURIComponent(newPhrase);
    window.open(tweetUrl);
  });
//_directories.js
/* Programs of Study */
  // $(".node-progam-of-study .options").text(function(index, text) {
  //     return text.replace('[node:field_options]', '--');
  // });

  // $(".node-progam-of-study .degrees").text(function(index, text) {
  //     return text.replace('[node:field_degrees]', '--');
  // });

  // $(".node-progam-of-study .campus").text(function(index, text) {
  //     return text.replace('[node:field_campus]', '--');
  // });

  $(".node-progam-of-study .col-xs-3").not(".STG, .UTSG, .UTM, .OS, .UTS, .UTSC, .OC").addClass("noCampus");

  //a to z, campus code
  $(".views-field.views-field-field-campus:contains('UTSG')").replaceWith("<span class='atoz dt'>Downtown Toronto</span>");
  $(".views-field.views-field-field-campus:contains('UTM')").replaceWith("<span class='atoz m'>Mississauga</span>");
  $(".views-field.views-field-field-campus:contains('UTSC')").replaceWith("<span class='atoz s'>Scarborough</span>");
  $(".views-field.views-field-field-campus:contains('OC')").replaceWith("<span class='atoz oc'>Off Campus</span>");
  $(".views-field.views-field-field-campus:contains('OS')").replaceWith("<span class='atoz os'>Online Services</span>");
  $(".directory.atoz .attachment.attachment-before .view-content .views-summary.views-summary-unformatted:last-child").after("<span class='resetBtn'><a href='/directory/a-to-z/'>All <i class='fa fa-check'></i></a></span>");

  //programs of study 
  $(".view-content .node-progam-of-study .campus:contains('STG')").replaceWith("<p class='campus'>Downtown Toronto</p>");
  $(".node-atoz .campus:contains('UTSG')").replaceWith("<p class='campus'>Downtown Toronto</p>");
  $(".view-content .node-progam-of-study .campus:contains('UTM')").replaceWith("<p class='campus'>Mississauga</p>");
  $(".node-atoz .campus:contains('UTM')").replaceWith("<p class='campus'>Mississauga</p>");
  $(".view-content .node-progam-of-study .campus:contains('UTSC')").replaceWith("<p class='campus'>Scarborough</p>");
  $(".node-atoz .campus:contains('UTSC')").replaceWith("<p class='campus'>Scarborough</p>");
  //search result
  $(".group-result-progam-of-study .col-md-3 .campus:contains('STG')").replaceWith("<p class='campus'>Downtown Toronto</p>");
  $(".group-result-progam-of-study .col-md-3 .campus:contains('UTM')").replaceWith("<p class='campus'>Mississauga</p>");
  $(".group-result-progam-of-study .col-md-3 .campus:contains('UTSC')").replaceWith("<p class='campus'>Scarborough</p>");
  $(".group-result-progam-of-study .col-md-3 .options:contains('N')").hide();

  //hide new line
  $(".view-content .node-progam-of-study .options:contains('N')").hide();
  $(".view-programs-of-study #edit-jump .options:contains('N')").hide();


  $(".view-programs-of-study #edit-jump option:contains('Master')").hide();
  $(".view-programs-of-study #edit-jump option:contains('Doctor')").hide();

  $(":radio[value=1]").addClass("undergrad");
  $(".view-programs-of-study .undergrad").click(function() {
    $(".view-programs-of-study #edit-jump option:contains('Bachelor')").show();
    $(".view-programs-of-study #edit-jump option:contains('Master')").hide();
    $(".view-programs-of-study #edit-jump option:contains('Doctor')").hide();
    window.location = '/academics/programs-directory?field_degrees_value=1&keys=';
  });

  $(":radio[value=2]").addClass("grad");
  $(".view-programs-of-study .grad").click(function() {
    window.location = '/academics/programs-directory?field_degrees_value=2&keys=';
  });

  if ($(":radio[value=2]:checked").val()) {
    $(".view-programs-of-study #edit-jump option:contains('Bachelor')").hide();
    $(".view-programs-of-study #edit-jump option:contains('Master')").show();
    $(".view-programs-of-study #edit-jump option:contains('Doctor')").show();
  }

  //new Programs Directory code
  //remove -Any- button
  $(window).ready(function() {
    $(".page-academics-programs-of-study .programs-directory .view-filters #edit-category .form-item:first-of-type").remove();
    $(".page-academics-programs-of-study .programs-directory .view-header, .page-academics-programs-of-study .programs-directory .view-filters").wrapAll("<div id='programs-header' class='clearfix'></div>");
    $(".page-academics-programs-of-study .programs-directory .view-filters .views-submit-button").insertBefore(".page-academics-programs-of-study .programs-directory .view-filters #edit-category-wrapper");
    $(".page-academics-programs-of-study .programs-directory .boilerplate").insertAfter(".page-academics-programs-of-study #page-title")
    //filter on click
    $("#edit-category .grad").click(function() {
      window.location = '/academics/programs-of-study?search_api_views_fulltext=&category=2';
    });
    //filter on click - undergrad
    $("#edit-category .undergrad").click(function() {
      window.location = '/academics/programs-of-study?search_api_views_fulltext=&category=1';
    });
    //add placeholder text
    $("#edit-search-api-views-fulltext-wrapper input").attr("placeholder", "Search by location, specialty, degree or program");

    if ($(".form-radio.grad").is(":checked")) {
      $(".page-academics-programs-of-study #edit-category .form-item:last-of-type").addClass("active");
      $(".page-academics-programs-of-study").addClass("grad");
      $("<h3 class='grad-title'>Graduate programs</h3>").insertAfter(".page-academics-programs-of-study.grad #programs-header");
    } else {
      $(".page-academics-programs-of-study #edit-category .form-item:last-of-type").removeClass("active");
      $(".page-academics-programs-of-study").removeClass("grad");
      $(".grad-title").remove();
    }
    if ($(".form-radio.undergrad").is(":checked")) {
      $(".page-academics-programs-of-study #edit-category .form-item:first-of-type").addClass("active");
      $(".page-academics-programs-of-study").addClass("undergrad");
      $("<h3 class='undergrad-title'>Undergraduate programs</h3>").insertAfter(".page-academics-programs-of-study.undergrad #programs-header");
    } else {
      $(".page-academics-programs-of-study #edit-category .form-item:first-of-type").removeClass("active");
      $(".page-academics-programs-of-study").removeClass("undergrad");
      $(".undergrad-title").remove();
    }

  });

  //blue book entry
  $(".node-bluebook .bluebook-entry .location:contains('St.George')").replaceWith("<p class='location'>Downtown Toronto</p>");
  $(".node-bluebook .bluebook-entry .location:contains('U of T Scarborough')").replaceWith("<p class='location'>Mississauga</p>");
  $(".node-bluebook .bluebook-entry .location:contains('U of T Mississauga')").replaceWith("<p class='location'>Scarborough</p>");


  $(".programs-directory .attachment .form-item.form-type-select").after("<span class='resetBtn'><a href='/academics/directory/programs-of-study/?keys=&field_degrees_value=1'>Reset</a></span>"); // find out how to add this button from within Drupal - not great as is

  //removes empty node
  $(".front-banner .caption_wrapper .caption .caption-body:contains('[node:body]')").hide();

  //adds container to search pages
  $(".page-search .block.block-system.block-system-main").addClass("container");
  $(".page-search .block.block-xml-search.block-xml-search-xml-search").addClass("container");
  $(".search-drawer-wrapper .search-form .form-radios.custom-search-selector").addClass("clearfix"); //fix float, if there's a better place to assign this class, we should 

  $(".search-drawer-wrapper .search-form .option:contains('AtoZ')").replaceWith("A-Z Directory");
  $(".search-drawer-wrapper .search-form .option:contains('Program of Study')").replaceWith("Programs of Study");
  $(".search-drawer-wrapper .search-form .option:contains('Story')").replaceWith("News Stories");


//Directories - search field 
  $('.programs-directory .view-filters .views-widget-filter-keys .form-type-textfield .form-text').focus(function() {
    $('.programs-directory .views-widget-filter-keys .form-text').attr('value', ' ');
  });
  //Link to all
  $("<a style='padding-left: 40px; text-decoration: underline;' href='/academics/programs-directory/all'>Show all</p>").insertAfter(".pane-programs-of-study .view-programs-of-study .pager .last");
  //clearfix on all - bug to investigate
  $(".view-programs-of-study .view-content").addClass("clearfix");

  //change action on search to print /search/content/Search
  //submitVal = $('#custom-search-blocks-form-1 .form-submit').val(); 
  $("#custom-search-blocks-form-1").attr("action", '/search/content/');


    $(".tiles .views-row .m").parent().css({ 'background-color': '#07766c' });
  $(".tiles .views-row .s").parent().css({ 'background-color': '#5B6BC8' });
  $(".tiles .views-row span:contains('STG')").replaceWith("Downtown Toronto");
//_mobile_menu.js
/* Mobile Menu ==== hide/show bars */
  $('.mobile .fa-bars').click(function() {
    $('.off-canvas-wrapper').slideDown(400);
    $('.mobile .fa.fa-times').fadeIn().addClass('rubberBand');
    $('.mobile .fa.fa-bars').hide();
    // add the class to hide the scroll bars on html - hideDoubleScroll found in _newHeader
    $('html').addClass('hideDoubleScroll');
  });
  $('.mobile .fa-times').click(function() {
    $('.off-canvas-wrapper').slideUp(400);
    $('.mobile .fa.fa-bars').fadeIn().addClass('rubberBand');
    $('.mobile .fa.fa-times').hide();
    // remove the class to hide the scroll bars on html - hideDoubleScroll found in _newHeader
    $('html').removeClass('hideDoubleScroll');
  });
//_header.js
/*NEW HEADER */
/* Drops the menu down --- TODO: make this smoother --- */
$('header .section .row .right-menu .menu li .jump-to').click(function() {
  var innermenu = $("header .section .row .right-menu .menu li.first .menu");
  var jumpmenu = $("header .section .row .right-menu .menu li .jump-to");
  innermenu.show(200, function() {
    innermenu.fadeIn();
  });
  jumpmenu.css('width', '100%');
});
/* Click anywhere, cause nav to slide up --- */
$(document).mouseup(function(e) {
  var innermenu = $("header .section .row .right-menu .menu li.first .menu");
  var jumpmenu = $("header .section .row .right-menu .menu li .jump-to");
  if (!innermenu.is(e.target) && innermenu.has(e.target).length === 0) {
    innermenu.hide(200, function() {
      innermenu.fadeOut();
    });
    jumpmenu.css('width', 'auto');
  }
});

var announcementClose = $(".head-announcement");

// Minimize the header height on scroll, show surveys
$(window).scroll(function() {
  var scrollTop = $(document).scrollTop();
  var windowWidth = $(window).width();
  var siteLogo = $(".header .site-logo-large");

  if (scrollTop > 300 && windowWidth >= 1024) {
    siteLogo.css("width", "75%");
    announcementClose.slideUp();
  } else {
    siteLogo.css("width", "100%");
    announcementClose.slideDown();
  }

});

$(".head-announcement #close").click(function() {
  announcementClose.remove();
})

//TAKEOVER BTN

if ($('#sub-menu-wrapper .btn-takeover').length) {
  $('#sub-menu-wrapper').addClass('active takeover')
}

//_convocation.js
var $contoggle = $('button[aria-controls="morecontent"]');
$contoggle.click(function(e) {
	e.preventDefault();
	$contoggle.removeClass('showthis');
	$(this).siblings( "button" ).addClass('showthis');   
}); 
//_admin.js
//ADMIN screen 
  $('body.page-user #user-login').addClass('container');
  $('body.page-user .section h1.title').addClass('container');
//_geo.js
  //Geolocation Banner
  $(".page-home .view-home-banner div[data-country]").each(function() {
    var parent = $(this).parent(".views-row");
    parent.addClass('geolocation');
    var country = $(this).data('country');
    if (country) {
      var split = country.toLowerCase().split(',');
      for (i = 0; i < split.length; i++) {
        var format = split[i].trim().replace(/\W/g, '-');
        parent.addClass(format);
      }
    }
    var region = $(this).data('region');
    if (region) {
      var split2 = region.toLowerCase().split(',');
      for (i = 0; i < split2.length; i++) {
        var format = split2[i].trim().replace(/\W/g, '-');
        parent.addClass(format);
      }
    }
    var city = $(this).data('city');
    if (city) {
      var split3 = city.toLowerCase().split(',');
      for (i = 0; i < split3.length; i++) {
        var format = split3[i].trim().replace(/\W/g, '-');
        parent.addClass(format);
      }
    }

  });

  //randomize the banner
  $(document).ready(function() {
    $('.news-home-banner .youtube').siblings(".title-header").css({ 'background-color': '#fff', 'opacity': '.7' });
    var geo = $('.page-home .view-home-banner').find('.views-row.geolocation');
    var geolen = geo.length;
    var divs = $('.page-home .view-home-banner').find('.views-row:not(.geolocation)'),
      len = divs.length, //number of divs on page
      randomDiv, //random number that will be generated
      randomDiv = Math.floor(Math.random() * len);
    divs.removeClass('show');
    divs.eq(randomDiv).addClass('show');

    if (geolen >= 1) {
      var nocache = new Date().getTime();
      $.get("https://ipinfo.io?token=4153ce3b33bbb9&cache=" + nocache, function(response) {
        var geocity = response.city.toLowerCase().replace(/\W/g, '-');
        var georegion = response.region.toLowerCase().replace(/\W/g, '-');
        var geocountry = response.country.toLowerCase().replace(/\W/g, '-');

        if (geo.hasClass(geocountry)) {
          if (geo.hasClass(georegion)) {
            var geotarget = $('.page-home .view-home-banner').find('.views-row.geolocation.' + geocountry + "." + georegion),
              lenr = geotarget.length, //number of divs on page
              randomDivr,
              randomDivr = Math.floor(Math.random() * lenr);
            geo.removeClass('show');
            divs.removeClass('show');
            geotarget.eq(randomDivr).fadeIn("slow", function() { $(this).addClass('show'); });

          } else if (geo.hasClass(geocity)) {
            var geotarget2 = $('.page-home .view-home-banner').find('.views-row.geolocation.' + geocountry + "." + geocity),
              lenc = geotarget2.length, //number of divs on page
              randomDivc,
              randomDivc = Math.floor(Math.random() * lenc);
            geo.removeClass('show');
            divs.removeClass('show');
            geotarget2.eq(randomDivc).fadeIn("slow", function() { $(this).addClass('show'); });
          }
          else if (!geo.hasClass(geocity) && !geo.hasClass(georegion)) {
            var geotarget3 = $('.page-home .view-home-banner').find('.views-row.geolocation.' + geocountry),
              lenc = geotarget3.length, //number of divs on page
              randomDivc,
              randomDivc = Math.floor(Math.random() * lenc);
            geo.removeClass('show');
            divs.removeClass('show');
            geotarget3.eq(randomDivc).fadeIn("slow", function() { $(this).addClass('show'); });
          }

        }
      }, "jsonp");

    }
  });
//_search_nav.js
// Search drop down drawer, click anywhere slide up
  var searchDrawer = $(".search-drawer-wrapper");

  searchDrawer.hide();
  $('a.search-mobile').click(function() {
    //event.preventDefault();
    $(".search-drawer-wrapper:not(.page-search .search-drawer-wrapper)").toggle("fast");
    return false;
  });
  $('a.search').click(function() {
    $(".search-drawer-wrapper:not(.page-search .search-drawer-wrapper)").toggle("fast");
    return false;
  });

  $(document).click(function(e) {
    if ($(e.target).closest('.search-drawer-wrapper').length === 0) {
      searchDrawer.hide(500, function() {
        searchDrawer.slideUp();
      });
    }
  });

  //scroll to top of page when search clicked
  $('.page-search a.search').click(function() {
    $("html, body").animate({ scrollTop: 0 }, "slow");
    return false;
  });

  // UofT Search, remove break
  // $("#xml-search-result .xml-search-item .body:contains('<br></br>')").each(function(){
  //   $(this).html($(this).html().split("<br></br>").join(""));
  // });
  $("#xml-search-result .xml-search-items-list .body br").remove();

//_social_media.js
// Social Media Directory pages 
  var socialTab = $(".page-social-media-directory .tabs");
  var socialContent = $(".page-social-media-directory .region.region-content .view-social-media-directory-tabs .view-content");
  socialTab.insertBefore(socialContent);
  //replace urls with FA icons
  var fbDirectory = $(".views-field-field-fb-url .field-content a:contains('facebook')");
  var fbEntry = $(".node-social-media-entry p:contains('facebook')");
  //fbDirectory.html('<i class="fa fa-facebook"></i>');
  if (fbDirectory || fbEntry) {
    fbEntry.html('<i class="fa fa-facebook"></i>');
    fbDirectory.html('<i class="fa fa-facebook"></i>');
  }
  var igDirectory = $(".views-field-field-ig-url .field-content a:contains('instagram')");
  var igEntry = $(".node-social-media-entry p:contains('instagram')");
  if (igDirectory || igEntry) {
    igDirectory.html('<i class="fa fa-instagram"></i>');
    igEntry.html('<i class="fa fa-instagram"></i>');
  }
  var liDirectory = $(".views-field-field-linkedin-url .field-content a:contains('linkedin')");
  var liEntry = $(".node-social-media-entry p:contains('linkedin')");
  if (liDirectory || liEntry) {
    liDirectory.html('<i class="fa fa-linkedin"></i>');
    liEntry.html('<i class="fa fa-linkedin"></i>');
  }
  var ytDirectory = $(".views-field-field-youtube-url .field-content a:contains('youtube')");
  var ytEntry = $(".node-social-media-entry p:contains('youtube')");
  if (ytDirectory || ytEntry) {
    ytDirectory.html('<i class="fa fa-youtube"></i>');
    ytEntry.html('<i class="fa fa-youtube"></i>');
  }
  var twitterDirectory = $(".views-field-field-twitter-url .field-content a:contains('twitter')");
  var twitterEntry = $(".node-social-media-entry p:contains('twitter')");
  if (twitterDirectory || twitterEntry) {
    twitterDirectory.html('<i class="fa fa-twitter"></i>');
    twitterEntry.html('<i class="fa fa-twitter"></i>');
  }
  //remove empty divs containing no social icons
  $(".views-field .field-content:empty").addClass('removeMe');
  if ($(".views-field").find(".removeMe").length > 0) {
    $(".removeMe").parent().remove();
  }
  //replace text
  // $(".view-social-media-directory-tabs .views-submit-button .form-submit").css("font-family", "FontAwesome");
  $(".view-social-media-directory-tabs .views-submit-button .form-submit").attr("value", "");
  $(".view-social-media-directory-tabs .views-widget-filter-keys .form-type-textfield .form-text").attr("placeholder", "Example: Medicine");
  $(".view-social-media-directory-tabs .view-filters").before("<h3>Search for schools, departments, or topics:</h3>");

//_campus_status.js
//new code
  //custom type: ok
  $(".campus-status-wrapper .col-sm-4 .ok .stat-title + .field .field-item").before("<div class='greenCirc'></div>");
  if ($(".campus-status-wrapper .col-sm-4 .status-body").hasClass("ok")) {

    if ($(".campus-status-wrapper .col-sm-4").find(".ok").length > 0) {
      $(".ok").parent().addClass("statusOpen clearfix");
      $(".statusOpen .msg-body").addClass("openBody");
    }
  }

  //custom type: standy
  $(".campus-status-wrapper .col-sm-4 .standby .stat-title + .field .field-item").before("<div class='orangeCirc'></div>");

  if ($(".campus-status-wrapper .col-sm-4 .status-body").hasClass("standby")) {

    if ($(".campus-status-wrapper .col-sm-4").find(".standby").length > 0) {
      $(".standby").parent().addClass("statusStandby clearfix");
      $(".statusStandby .msg-body").addClass("standbyBody");
    }

  }

  //in custom type: select list with alert
  $(".campus-status-wrapper .col-sm-4 .alert .stat-title + .field .field-item").before("<div class='redCirc'></div>");

  if ($(".campus-status-wrapper .col-sm-4 .status-body").hasClass("alert")) {

    if ($(".campus-status-wrapper .col-sm-4").find(".alert").length > 0) {
      $(".alert").parent().addClass("statusClosed clearfix");
      $(".statusClosed .msg-body").addClass("closedBody");
    }
  }


  //Main page/footer
  var campusBarOpen = $(".campusStatus .pane-campus-status .pane-content .field-content:contains('ok')");
  var campusFooterOpen = $(".campusStatusFooter .views-field-field-main-campus-status .field-content:contains('ok')");
  if (campusBarOpen || campusFooterOpen) {

    campusBarOpen.replaceWith("<a href='/campus-status' alt='campus is ok'><i class='fa fa-check-circle green okLevel' aria-hidden='true'></i></a>");
    campusFooterOpen.replaceWith("<a href='/campus-status' alt='campus is ok'><i class='fa fa-check-circle green okLevel' aria-hidden='true'></i></a>");

    if ($(".campusStatus .pane-campus-status .pane-content .views-field-field-main-campus-status i").hasClass("okLevel")) {
      $(".campusStatus .pane-campus-status .pane-title a").css("color", "#4CAF50");
      //$(".campusStatus .pane-campus-status .pane-content").addClass("circleEmerg");
    }

  }
  var campusBarStandby = $(".campusStatus .pane-campus-status .pane-content .field-content:contains('standby')");
  var campusFooterStandby = $(".campusStatusFooter .views-field-field-main-campus-status .field-content:contains('standby')");
  if (campusBarStandby || campusFooterStandby) {

    campusBarStandby.replaceWith("<a href='/campus-status' alt='campus is on standby'><i class='fa fa-exclamation-circle orange standbyLevel' aria-hidden='true'></i></a>");
    campusFooterStandby.replaceWith("<a href='/campus-status' alt='campus is standby'><i class='fa fa-exclamation-circle orange standbyLevel' aria-hidden='true'></i></a>");
    if ($(".campusStatus .pane-campus-status .pane-content .views-field-field-main-campus-status i").hasClass("standbyLevel")) {
      $(".campusStatus .pane-campus-status .pane-title a").css("color", "#FFE498");
      //$(".campusStatus .pane-campus-status .pane-content").addClass("circleEmerg");
    }

  }

  var campusBarClosed = $(".campusStatus .pane-campus-status .pane-content .field-content:contains('alert')");
  var campusFooterClosed = $(".campusStatusFooter .views-field-field-main-campus-status .field-content:contains('alert')");
  if (campusBarClosed || campusFooterClosed) {
    campusBarClosed.replaceWith("<a href='/campus-status' alt='campus is on alert'><i class='fa fa-flag red alertLevel' aria-hidden='true'></i></a>");
    campusFooterClosed.replaceWith("<a href='/campus-status' alt='campus is on alert'><i class='fa fa-flag red alertLevel' aria-hidden='true'></i></a>");
    if ($(".campusStatus .pane-campus-status .pane-content .views-field-field-main-campus-status i").hasClass("alertLevel")) {
      $(".campusStatus .pane-campus-status .pane-title a").css("color", "#F64747");
      $(".campusStatus .pane-campus-status .pane-content").addClass("circleEmerg");
    }

  }

  //TODO - emergency (high-alert, icon with white background, red flag)
  var campusBarEmerg = $(".campusStatus .pane-campus-status .pane-content .field-content:contains('emergency')");
  var campusFooterEmerg = $(".campusStatusFooter .views-field-field-main-campus-status .field-content:contains('emergency')");
  if (campusBarEmerg || campusFooterEmerg) {
    campusBarEmerg.replaceWith("<a href='/campus-status'><i class='fa fa-flag red emerg' aria-hidden='true'></i></a>");
    campusFooterEmerg.replaceWith("<a href='/campus-status'><i class='fa fa-flag red emerg' aria-hidden='true'></i></a>");
    if ($(".campusStatus .pane-campus-status .pane-content .views-field-field-main-campus-status i").hasClass("emerg")) {
      $(".campusStatus .col-md-3").css("background-color", "#F64747");
      $(".campusStatus .pane-campus-status .pane-content").addClass("circleEmerg");
    }

  }

  $(".campusStatusFooter.footer-5cols .view-content .views-field .fa").before("<p class='campusStatusLabel'><a href='/campus-status'>Campus Status: </a></p>");

  /* Announcements bar
   ************************************************/
  var campusHeight = $(".campusStatus .col-md-9 .pane-content").height();
  if (campusHeight > 60) {
    $(".campusStatus .col-md-9 .pane-content").css("height", "60px");
    $("<i class='fadeIn fa fa-chevron-down' aria-hidden='true' style='color:white; position: absolute; top: 35px; right: 3px; cursor: chevron;'></i>").insertAfter(".campusStatus .col-md-9 .pane-content p:first-child");
  }
  $(".campusStatus .col-md-9 .pane-content i").click(function() {
    $('.campusStatus .col-md-9 .pane-content').scrollTop($('.campusStatus .col-md-9 .pane-content')[0].scrollHeight);
  });

  $('.campusStatus .col-md-9 .pane-content p:contains("emptyAnnouncementsBox")').parent().addClass("hideAnnouncement");
  if ($('.campusStatus .col-md-9 .pane-content').hasClass("hideAnnouncement")) {
    $(".hideAnnouncement").parent().addClass("hideAnnouncementParent");
    $(".hideAnnouncementParent").parent().addClass("hideAnnouncementCont");
  }

  //Announcement banner overlay
  $(window).load(function() {
    $('.banner-teaser').addClass('bannerDown').delay(800);
  });
  /* END Announcements bar ************************************************/

//_utw.js
  //uoft world blocks


  $(window).load(function() {
    var maxHeightUTW = -1;
    var uoftWorldBlocks = $(".view-u-of-t-world .uoftworld-row");
    uoftWorldBlocks.each(function() {
      maxHeightUTW = maxHeightUTW > $(this).height() ? maxHeightUTW : $(this).height();
    });

    uoftWorldBlocks.each(function() {
      $(this).height(maxHeightUTW);
    });
  });


   /* U of T World - v2
   ************************************************/

  //add clearfixes to view-header and view-content
  $(".view-u-of-t-world .view-header").addClass("clearfix");
  $(".view-u-of-t-world .view-content").addClass("clearfix");


  //.ready prevents the footer from loading before the view-content is ready
  $(window).ready(function() {
    //move view-footer out of view so that it goes 100% width
    $(".view-u-of-t-world .view-footer").insertAfter("#block-system-main .utw-content");
    $(".view-u-of-t-world ").addClass("animated fadeIn");
    //check to see that this is an inner UTW page, filtered by attachment
    if ($("body[class*='page-uoft-world-']").length) {
      $("#block-system-main .view-footer").hide();
    }
    //check to see that this is an Edition of UTW, if yes, show bottom nav
    if ($("body[class*='page-uoft-world-editions']").length) {
      $("#block-system-main .view-footer").show();
    }

    //add previous and next edition buttons
    if ($("body[class*='page-uoft-world']").length) {
      $("<span>Previous Edition</span>").insertBefore(".page-uoft-world #main .view-footer .menu .first.leaf a");
      $("<h3>Want more U of T World?</h3>").insertBefore(".page-uoft-world #main .view-footer .menu .first.leaf + .leaf a");
      $(".page-uoft-world #main .view-footer .menu .first.leaf + .leaf").addClass("middle-leaf");
      $("<span>Next Edition</span>").insertBefore(".page-uoft-world #main .view-footer .menu .last.leaf a");
    }

    if ($("body[class*='page-uoft-world-from-publication']").length) {
      $("<div class='view-footer'><ul class='menu'><li class='archive leaf'><h3>Want more U of T World?</h3><a href='/uoft-world/archives'><span>Explore</span></a></li></ul></div>").insertAfter(".page-uoft-world-from-publication #main .utw-content");
    }

    //wrap text in button style
    $(".page-uoft-world #main .view-footer .menu .middle-leaf a").wrapInner("<span></span>");

    //if has -special in class, hide From Publication
    if ($(".page-uoft-world div[class*='-special']").length) {
      $(".uoftworld-row .node-u-of-t-world .from-pub").remove();
    }
    //do the same for homepage
    if ($(".page-home div[class*='-special']").length) {
      $(".node-u-of-t-world-homepage .story-text .article-from").remove();
    }

    // if ($("body[class*='page-uoft-world-archives']").length) {

    //     $(".utw-content .view-content .views-field-field-utw-edition .field-content a").each(function(index) {
    //         var str = $(this).attr('href');
    //         $(".utw-content .view-content .views-field-field-utw-edition span").each(function(index) {
    //             $(this).wrapInner("<a href='" + str + "'></a>");
    //         });
    //     });
    // }
    // var href = $(this).attr('href');

  });

  $("<li class='mobile leaf'><h3>Want more U of T World?</h3><a href='/uoft-world/archives'><span>Explore</span></a></li>").insertBefore(".utw-content .block-menu .menu .first.leaf");
  $(window).bind('resize', function(event) {
    //Whatever you want to run here     
    if ($(window).width() <= 768) {
      $(".page-uoft-world #main .view-footer .menu .first.leaf").show();
      $(".page-uoft-world #main .view-footer .menu .first.leaf").css("visibility", "visible");
      //$(".page-uoft-world #main .view-footer .menu li:nth-child(3)").hide();
    }
  });


  //});
//_entrepreneurs.js

// $(document).load(function(){
//   var windowWidth = $(window).width();
//   if (windowWidth < 990) {
//     $('#schedule .img-ent').after('#schedule .title-ent');
//     console.log("schedule img shift");
//   }
// })

$(window).bind('resize', function(event) {

  if ($(window).width() <= 990) {
    $('#courses .img-ent').insertAfter('#courses .title-ent');
    $('#schedule .img-ent').insertAfter('#schedule .title-ent');

  } else {
    $('#courses .img-ent').insertAfter('#courses .body-ent');
    $('#schedule .img-ent').insertAfter('#schedule .body-ent');
  }

})

var $togglebutton = $('button[aria-controls="morestories"]');
$togglebutton.click(function(e) {
	e.preventDefault();
	$togglebutton.removeClass('showthis');
  $('#moreEntNews').removeClass('hide');
  $('#moreEntNews').addClass('showthis');
	//$(this).siblings( "button" ).addClass('showthis');
});

var $togglebutton1 = $('button[aria-controls="morestartup"]');
$togglebutton1.click(function(e) {
	e.preventDefault();
	$togglebutton1.removeClass('showthis');
	$(this).siblings( "button" ).addClass('showthis');
});

// FOR startup Stories page
if ($("body").hasClass("story-page")) {
  if($('#story-type').length) {

    var storyType = $.trim($('#story-type').find('.pane-content').html());
    if (storyType == 'startup') {
      $('#content').addClass('ent-outer-wrapper');
    }
  }
}


//END JQUERY
});
//SITE FEEDBACK SET USER AGENT AND REFERAL URL

jQuery(function($) {

  $(document).ready(function() {

    if ($("body").hasClass("site-feedback-form")) {
      var referrer =  document.referrer;
      var userAgent = navigator.userAgent;

      var referrerURL = 'https://www.utoronto.ca/site-feedback';

      if (referrer!='' && typeof referrer != "undefined") { // SET REFERER URL

        referrerURL = referrer;
      }

      if ($('input[name=\"submitted[page_you_are_reffering_to]\"]').val()=='') { //ONLY CHANGE IF FIELD IS EMPTY

        $('input[name=\"submitted[page_you_are_reffering_to]\"]').val(referrerURL);
      }

      if ($('input[name=\"submitted[field_1]\"]').val()=='') { //ONLY CHANGE IF FIELD IS EMPTY

        $('input[name=\"submitted[field_1]\"]').val(referrerURL);
      }

      if (userAgent!='' && typeof referrer != "undefined") { // SET USER AGENT URL
        $('input[name=\"submitted[field_2]\"]').val(userAgent);
      }

    }
  });
});
;
